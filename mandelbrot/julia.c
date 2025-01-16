#include <X11/Xlib.h>
#include <X11/keysym.h>  // Include this header for key symbols
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define WIDTH 800
#define HEIGHT 600
#define MAX_ITER 256

// Function to map iteration count to a color
unsigned long get_color(int iter) {
    if (iter == MAX_ITER) return 0; // Black for points in the set

    double t = (double)iter / MAX_ITER;
    int r = (int)(9 * (1 - t) * pow(t, 3) * 255);
    int g = (int)(15 * pow((1 - t), 2) * pow(t, 2) * 255);
    int b = (int)(8.5 * pow(1 - t, 3) * t * 255);

    return (r << 16) | (g << 8) | b;
}

// Function to compute and render the Julia set
void render_julia(Display* display, Window window, Pixmap pixmap) {
    GC gc = DefaultGC(display, 0);
    double x1 = -1.5, y1 = -1, x2 = 1.5, y2 = 1;
    double zx, zy, cX = -0.7, cY = 0.27015;

    for (int row = 0; row < HEIGHT; row++) {
        for (int col = 0; col < WIDTH; col++) {
            zx = x1 + (x2 - x1) * col / WIDTH;
            zy = y1 + (y2 - y1) * row / HEIGHT;

            int iter = MAX_ITER;
            while ((zx * zx + zy * zy < 4.0) && (iter > 0)) {
                double tmp = zx * zx - zy * zy + cX;
                zy = 2.0 * zx * zy + cY;
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

    // Create an off-screen pixmap for double buffering
    Pixmap pixmap = XCreatePixmap(display, RootWindow(display, screen), WIDTH, HEIGHT, 24);

    Window window = XCreateSimpleWindow(display, RootWindow(display, screen), 0, 0,
                                       WIDTH, HEIGHT, 1, BlackPixel(display, screen),
                                       WhitePixel(display, screen));

    XStoreName(display, window, "Press ESC to Close");
    XSelectInput(display, window, KeyPressMask);

    // Map the window
    XMapWindow(display, window);
    
    // Manage the close button ( top right) : when pressed we do nothing 
    Atom wm_delete_window = XInternAtom(display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(display, window, &wm_delete_window, 1);

    // Render the Julia set onto the off-screen pixmap
    render_julia(display, window, pixmap);

    // Copy from the off-screen pixmap to the on-screen window in one go
    GC gc = DefaultGC(display, 0);
    XCopyArea(display, pixmap, window, gc, 0, 0, WIDTH, HEIGHT, 0, 0);
    XFlush(display);

    // Event loop to keep the window open until Escape is pressed
    int running = 1;
    while (running) {
        XEvent event;
        XNextEvent(display, &event);
        if (event.type == KeyPress) {
            KeySym keysym = XLookupKeysym(&event.xkey, 0);
            if (keysym == XK_Escape) { // Check for Escape key press
                running = 0;
            }
        }
    }

    // Clean up resources
    XFreePixmap(display, pixmap);
    XCloseDisplay(display);
    return 0;
}
