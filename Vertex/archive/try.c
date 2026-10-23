#include<stdio.h>
#include<X11/Xlib.h>
#include<X11/keysym.h>
#include<stdlib.h>
Display *dpy;

void main(int nb,char **val)
{
 KeySym modiflist[2];
 XEvent ev;
 XKeyboardControl clavier;
 GC gc;
 Window win;
 XGCValues gcv;
 int maxx,maxy,x1,y1,x2,y2;

 if(nb<2)
   {
    puts("Need One Argument");
    exit(0);
   }

 dpy = XOpenDisplay(val[1]);
 if(dpy == NULL)
   {
    puts("No Display");
    exit(0);
   }

 srand(time(0));
 win = DefaultRootWindow(dpy);

 maxx = WidthOfScreen(ScreenOfDisplay(dpy,DefaultScreen(dpy)));
 maxy = HeightOfScreen(ScreenOfDisplay(dpy,DefaultScreen(dpy)));

 gcv.function = GXcopy;
 gcv.line_width = 2;
 gcv.font = XLoadFont(dpy,"r14");

 gc = XCreateGC(dpy,win,GCFunction|GCFont,&gcv);

 XClearWindow(dpy,win);
 while(1)
      {
         x1 = random()%maxx;
         y1 = random()%maxy;
         x2 = random()%maxx;
         y2 = random()%maxy;
         XDrawLine(dpy,win,gc,x1,y1,x2,y2);
         usleep(20);
      } 
}
