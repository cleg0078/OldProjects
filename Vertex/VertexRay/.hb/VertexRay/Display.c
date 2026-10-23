#include<stdio.h>
#include"Connect.h"
#define d(x,y) ((((dm[((x) & 7) + (((y) & 7) << 3)] & 0xff)) - 128) / 3)

unsigned long color[216];
XGUI gui;
XShell *sh;
void (*AddPixel)(int,int,unsigned long,unsigned long,unsigned long);

void AddPixel8(int x,int y,unsigned long Red,unsigned long Green,unsigned long Blue)
{
 register  long thePixel;
 register int r, g, b, rr,gg,bb;
 register int luminence;
 static int dm[] = {
                    0, 192,  48, 240,  12, 204,  60, 252,
                    128,  64, 176, 112, 140,  76, 188, 124,
                    32, 224,  16, 208,  44, 236,  28, 220,
                    160,  96, 144,  80, 172, 108, 156,  92,
                    8, 200,  56, 248,   4, 196,  52, 244,
                    136,  72, 184, 120, 132,  68, 180, 116,
                    40, 232,  24, 216,  36, 228,  20, 212,
                    168, 104, 152,  88, 164, 100, 148,  84
                   };

 rr = ((int)Red  )+ d(x,y); if (rr>255) rr=255; if (rr<0) rr=0;
 gg = ((int)Green)+ d(x,y); if (gg>255) gg=255; if (gg<0) gg=0;
 bb = ((int)Blue )+ d(x,y); if (bb>255) bb=255; if (bb<0) bb=0;
 r = (rr * 216) / (256 * 36);
 g = (gg * 216) / (256 * 36);
 b = (bb * 216) / (256 * 36);

 /* find the pixel we got */
 thePixel = color[(r * 36) + (g * 6) + b];
 XSetForeground(sh->dpy,sh->gc,thePixel);
 XDrawPoint(sh->dpy,sh->Win,sh->gc,x,y);
}

void AddPixel16(int x,int y,unsigned long r,unsigned long g,unsigned long b)
{
 unsigned long rcol=(unsigned long )((unsigned long)(r>>3)<<11|
                    (unsigned long)(g>>2)<<5|(unsigned long)(b>>3));

 XSetForeground(gui.dpy,sh->gc,rcol);
 XDrawPoint(gui.dpy,sh->Win,sh->gc,x,y);
}

void AddPixel24(int x,int y,unsigned long r,unsigned long g,unsigned long b)
{
 unsigned long rcol = (unsigned long)((unsigned long)(r<<16) + (unsigned long)(g<<8) + (unsigned long)(b));  
 XSetForeground(gui.dpy,sh->gc,rcol);
 XDrawPoint(gui.dpy,sh->Win,sh->gc,x,y);
}
 
void WaitKeyBoard()
{
 WaitKey(sh);
}

void ClosePlot()
{
 DestroyShell(&gui,sh);
 XDeconnect(&gui);
}

int InitPlot(int w,int h)
{
 XColor tmp;
 short r, g, b, i, x, y;
 Colormap cmap;
 int mode;
 XWidget *entry;

 if( XConnect(&gui,"") == 0 )
   {
    fprintf(stderr,"No Display\n\n");
    return(0);
   }

 mode=DisplayPlanes(gui.dpy,DefaultScreen(gui.dpy));

 switch(mode)
       {
        case(24):
             AddPixel = &AddPixel24;
             printf("TRUE COLOR VISUAL: 24 Bits\n");
             break;
        case(15):
        case(16):
             AddPixel = &AddPixel16;
             printf("TRUE COLOR VISUAL: 16 Bits\n"); 
             break;
        case(8):
             AddPixel = &AddPixel8;
             cmap = DefaultColormap(gui.dpy, DefaultScreen(gui.dpy));
             for(i = r = 0; r < 6; r++)
                 for(g = 0; g < 6; g++)
                     for(b = 0; b < 6; b++, i++)
                        {
                         tmp.red = (r * 36) << 8;
                         tmp.green = (g * 36) << 8;
                         tmp.blue = (b * 36) << 8;
                         tmp.flags = DoRed | DoGreen | DoBlue;
                         if (!XAllocColor(gui.dpy, cmap, &tmp))
                            goto NotEnough;
                         color[i] = tmp.pixel;
                        }
             NotEnough:  
             if(i != 216)
                 fprintf(stderr, "warning: could only get %d colors.\n", i);

             printf("DIRECT COLOR VISUAL: 8 Bits\n");
             break;
        default:
             printf("Unsupported Display: %d Bits\n",mode);
             printf("You Need a 8 or 24 Bits Color Display\n");
             exit(0);
       }


 sh = CreateShell(&gui,"Vertex Ray By C.LEGENDRE",0,0,w,h);

 MapShell(sh);
 return(1);
}

         
