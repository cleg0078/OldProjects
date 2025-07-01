#include <X11/Xlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct {
    float x, y, z;
} Vec3;

Vec3 vec3(float x, float y, float z) {
    return (Vec3){x, y, z};
}

Vec3 normalize(Vec3 v) {
    float len = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    return vec3(v.x / len, v.y / len, v.z / len);
}

Vec3 trace_ray(Vec3 origin, Vec3 direction, int depth);

Vec3 reflect(Vec3 I, Vec3 N) {
    return vec3(I.x - 2.0f * (I.x * N.x + I.y * N.y + I.z * N.z) * N.x,
                I.y - 2.0f * (I.x * N.x + I.y * N.y + I.z * N.z) * N.y,
                I.z - 2.0f * (I.x * N.x + I.y * N.y + I.z * N.z) * N.z);
}

Vec3 trace_ray(Vec3 origin, Vec3 direction, int depth) {
    if (depth > 5) return vec3(0.0f, 0.0f, 0.0f);

    float t = 1e30;
    Vec3 obj_color = vec3(0.0f, 0.0f, 0.0f);
    Vec3 normal;

    // Sphere intersection
    {
        Vec3 oc = vec3(origin.x - 400.0f, origin.y - 300.0f, origin.z);
        float b = 2.0 * (oc.x * direction.x + oc.y * direction.y + oc.z * direction.z);
        float c = oc.x * oc.x + oc.y * oc.y + oc.z * oc.z - 1e4;
        float h = b * b - 4.0 * c;
        if (h > 0.0) {
            float temp = (-b - sqrt(h)) / 2.0;
            if (temp < t && temp > 1e-4) {
                t = temp;
                obj_color = vec3(1.0f, 0.5f, 0.31f);
                normal = normalize(vec3(origin.x + t * direction.x - 400.0f,
                                        origin.y + t * direction.y - 300.0f,
                                        origin.z + t * direction.z));
            }
        }
    }

    Vec3 hit_point = vec3(origin.x + t * direction.x, origin.y + t * direction.y, origin.z + t * direction.z);
    Vec3 reflected_color = trace_ray(hit_point, reflect(direction, normal), depth + 1);

    // Simple diffuse shading
    float light_dir_x = -0.2f;
    float light_dir_y = -1.0f;
    float light_dir_z = -0.3f;

    Vec3 light_direction = normalize(vec3(light_dir_x, light_dir_y, light_dir_z));
    float light_intensity = fmaxf(0.0f, normal.x * light_direction.x + normal.y * light_direction.y + normal.z * light_direction.z);
    Vec3 diffuse = vec3(obj_color.x * light_intensity, obj_color.y * light_intensity, obj_color.z * light_intensity);

    return vec3(diffuse.x * 0.1f + reflected_color.x * 0.9f,
                diffuse.y * 0.1f + reflected_color.y * 0.9f,
                diffuse.z * 0.1f + reflected_color.z * 0.9f);
}

void render(Display *display, Window window) {
    XImage *image = XCreateImage(display, DefaultVisual(display, 0), 24, ZPixmap, 0, NULL, 800, 600, 32, 0);

    unsigned long *data = (unsigned long *)malloc(800 * 600 * sizeof(unsigned long));

    for (int y = 0; y < 600; ++y) {
        for (int x = 0; x < 800; ++x) {
            float u = ((float)x / 800.0f - 0.5f) * 2.0f;
            float v = ((float)y / 600.0f - 0.5f) * 2.0f;

            Vec3 direction = normalize(vec3(u, v, -1.0f));
            Vec3 color = trace_ray(vec3(400.0f, 300.0f, 500.0f), direction, 0);

            data[y * 800 + x] = (unsigned long)(255 << 24 | ( (unsigned long)color.x * 255) << 16 | ( (unsigned long)color.y * 255) << 8 | (unsigned long)(color.z * 255) );
        }
    }

    image->data = (unsigned char *)data;
    XPutImage(display, window, DefaultGC(display, 0), image, 0, 0, 0, 0, 800, 600);

    //free(data);
    XDestroyImage(image);
}

int main() {
    Display *display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Unable to open X display\n");
        return 1;
    }

    int screen = DefaultScreen(display);
    Window window = XCreateSimpleWindow(display, RootWindow(display, screen), 0, 0, 800, 600, 1,
                                        BlackPixel(display, screen), WhitePixel(display, screen));

    XSelectInput(display, window, ExposureMask | KeyPressMask);
    XMapWindow(display, window);

    while (1) {
        XEvent event;
        XNextEvent(display, &event);

        if (event.type == Expose) {
            render(display, window);
        }

        if (event.type == KeyPress)
            break;
    }

    XCloseDisplay(display);
    return 0;
}
