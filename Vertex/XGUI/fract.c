#include"Connect.h"
#define MAXX 1980
#define MAXY 1080
#define MAXITER 256

XGUI gui;
XShell *sh;

int Lut[MAXITER];
void InitLut()
{
int i;
int depth;
XColor col;
int r, g, b;
float t;

depth = DefaultDepth(gui.dpy, DefaultScreen(gui.dpy));

if(depth == 1)
   {
    for(i=0;i<MAXITER;i++)
       Lut[i]=i%2;
   }
else
   {
    for(i=0;i<MAXITER;i++)
      {
       t = (float)i / MAXITER;
       r = (int)(255.0 * (0.5 + 0.5 * sin(3.14159 * t * 6.28)));
       g = (int)(255.0 * (0.5 + 0.5 * sin(3.14159 * t * 6.28 + 2.094)));
       b = (int)(255.0 * (0.5 + 0.5 * sin(3.14159 * t * 6.28 + 4.189)));
       
       col.red = r * 256;
       col.green = g * 256;
       col.blue = b * 256;
       col.flags = DoRed|DoGreen|DoBlue;
       XAllocColor(gui.dpy, DefaultColormap(gui.dpy, DefaultScreen(gui.dpy)),&col);
       Lut[i]=col.pixel;
      }
   }
}

void DrawFract()
{
int x,y,col;
float px,py,real,imag,real_new,imag_new,module,sx,sy;
float cx,cy;

cx=-0.7;
cy=0.27015;

sx= 4.0/MAXX;
sy= 4.0/MAXY;
py= -2.0-sy;
y=0;

while(y<MAXY)
    {
      px=-2.0-sx;
      py=py+sy;
      x=0;
      while(x<MAXX)
           {
            px=px+sx;
            real=px;
            imag=py;
            module=0;
            col=0;      
            while(module<4.0 && col<MAXITER )
                 {
                  real_new=real*real-imag*imag+cx;
                  imag_new=2*real*imag+cy;
                  real=real_new;
                  imag=imag_new;
                  module=real*real+imag*imag;
                  col=col+1;
                 }
              if(col<MAXITER)
                { 
                 Foreground(sh,Lut[col]); 
                 Putpixel(sh,x,y);
                } 
             else
              {
               Foreground(sh,0);
               Putpixel(sh,x,y);
              } 
             x++;
            }
        y++;
       }

}

void main()
{
 XConnect(&gui,"");

 sh = CreateShell(&gui,"Fractal Example By C.LEGENDRE",0,0,MAXX,MAXY);
 MapShell(sh);

  EnableDoubleBuffer(sh);

  InitLut();

  DrawFract();

 FlipBuffer(sh);
 
 WaitEscape(sh);
 XDeconnect(&gui);
 exit(0);
}

 
