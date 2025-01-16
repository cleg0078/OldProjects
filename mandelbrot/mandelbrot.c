#include <X11/Xlib.h>
#include <X11/keysym.h> 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#define WIDTH 800
#define HEIGHT 600
#define MAX_ITER 64
#define ZOOM_STEPS 1000
#define ZOOM_FACTOR 1.01

unsigned long get_color(int iter) {
    if (iter == MAX_ITER) return 0; // Black for points in the set

    double t = (double)iter / MAX_ITER;
    int r = (int)(9 * (1 - t) * pow(t, 3) * 255);
    int g = (int)(15 * pow((1 - t), 2) * pow(t, 2) * 255);
    int b = (int)(8.5 * pow(1 - t, 3) * t * 255);

    return (r << 16) | (g << 8) | b;
}

void render_mandelbrot(Display* display, Window window, Pixmap pixmap, double x1, double y1, double x2, double y2) {
    GC gc = DefaultGC(display, 0);

    for (int row = 0; row < HEIGHT; row++) {
        for (int col = 0; col < WIDTH; col++) {
            double zx = x1 + (x2 - x1) * col / WIDTH;
            double zy = y1 + (y2 - y1) * row / HEIGHT;

            int iter = MAX_ITER;
            while ((zx * zx + zy * zy < 4.0) && (iter > 0)) {
                double tmp = zx * zx - zy * zy + x1 + (x2 - x1) * col / WIDTH;
                zy = 2.0 * zx * zy + y1 + (y2 - y1) * row / HEIGHT;
                zx = tmp;
                iter--;
            }

            XSetForeground(display, gc, get_color(iter));
            XDrawPoint(display, pixmap, gc, col, row);
        }
    }
}

int main() {
    Display* display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Cannot open display\n");
        exit(1);
    }

    int screen = DefaultScreen(display);

    Pixmap pixmap = XCreatePixmap(display, RootWindow(display, screen), WIDTH, HEIGHT, 24);
    Window window = XCreateSimpleWindow(display, RootWindow(display, screen), 0, 0,
                                       WIDTH, HEIGHT, 1, BlackPixel(display, screen),
                                       WhitePixel(display, screen));

    // Set the window title
    XStoreName(display, window, "Press ESC to Close");

    XSelectInput(display, window, ExposureMask | KeyPressMask);
    XMapWindow(display, window);

    Atom wm_delete_window = XInternAtom(display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(display, window, &wm_delete_window, 1);

    // Initial view parameters
    double x1 = -2.0, y1 = -1.5, x2 = 1.0, y2 = 1.5;
    double zoom_center_x = (x1 + x2) / 3;
    double zoom_center_y = (y1 + y2) / 3;

    for (int step = 0; step < ZOOM_STEPS; step++) {
	XEvent event;

        if (XCheckTypedEvent(display, KeyPress, &event)) {
            if (event.xkey.keycode == XKeysymToKeycode(display, XK_Escape)) 
		    break;
        }

        // Zoom in
        double width = x2 - x1;
        double height = y2 - y1;
        x1 = zoom_center_x - width / ZOOM_FACTOR / 2.0;
        x2 = zoom_center_x + width / ZOOM_FACTOR / 2.0;
        y1 = zoom_center_y - height / ZOOM_FACTOR / 2.0;
        y2 = zoom_center_y + height / ZOOM_FACTOR / 2.0;

        // Render new fractal
        render_mandelbrot(display, window, pixmap, x1, y1, x2, y2);

        // Display the pixmap on the window
        GC gc = DefaultGC(display, screen);
        XCopyArea(display, pixmap, window, gc, 0, 0, WIDTH, HEIGHT, 0, 0);
        XFlush(display);

        usleep(500); // Sleep for a short time to create animation effect
    }

    XDestroyWindow(display, window);
    XCloseDisplay(display);

    return 0;
}
