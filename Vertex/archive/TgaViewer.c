#include<stdio.h>
#include"Connect.h"
#define d(x,y) ((dm[((x) & 7) + (((y) & 7) << 3)] & 0xff - 128) / 3)

unsigned long color[216];
XGUI gui;
XShell *sh,*out,*out2;
void (*AddPixel)(int,int,unsigned long,unsigned long,unsigned long);
XImage *im;

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
 XPutPixel(im,x,y,thePixel);
}

void AddPixel16(int x,int y,unsigned long r,unsigned long g,unsigned long b)
{
 unsigned long rcol=(unsigned long )((unsigned long)(r>>3)<<11|(unsigned long)(g>>2)<<5|(unsigned long)(b>>3));
 XPutPixel(im,x,y,rcol);
}

void AddPixel24(int x,int y,unsigned long r,unsigned long g,unsigned long b)
{
 register unsigned long rcol = (unsigned long)((unsigned long)(r<<16) + (unsigned long)(g<<8) + (unsigned long)(b));  
 XPutPixel(im,x,y,rcol);
}
 

void Quit()
{
 DestroyShell(&gui,sh);
 XDeconnect(&gui);
 exit(0);
}


void TGAOpen()
{
 Colormap cmap;
 XSetWindowAttributes attrs;
 FILE *f;
 register unsigned char *name,*buff,*data;
 unsigned char c,c1[12],yd,yr,xd,xr;
 register int pad,i,x,y,xpos,ypos; 
 int t0,t1;

 name = FileRequest(&gui,"Load TGA File",0,0,320,200);

 if( (f = fopen(name,"rb")) == NULL)
    return;

 fscanf(f,"%12c",c1); 
 fscanf(f,"%c",&c); xr = c;
 fscanf(f,"%c",&c); xd = c;
 fscanf(f,"%c",&c); yr = c;
 fscanf(f,"%c",&c); yd = c;

 fscanf(f,"%c",&c); 
 if( c!= 24 ) { fclose(f); return; }
 fscanf(f,"%c",&c); 
 if( c!= 0x20 ) { fclose(f); return; }

 x = xd*256 + xr;
 y = yd*256 + yr;

 out = CreateShell(&gui,"View File",0,0,x,y);
 
 switch( DisplayPlanes(gui.dpy,DefaultScreen(gui.dpy)) )
        {
         case(8):  data = (unsigned char *)malloc(x*y);
                   pad=8;
                   break;
         case(16): data = (unsigned char *)malloc(2*x*y);
                   pad=16;
                   break;
         case(24): data = (unsigned char *)malloc(3*x*y); 
                   pad = 32;
        }

 im=NULL;
 im = XCreateImage(gui.dpy, DefaultVisual(gui.dpy,DefaultScreen(gui.dpy)), 
                   DisplayPlanes(gui.dpy,DefaultScreen(gui.dpy)),
                   ZPixmap, 0, data,x,y,pad,x*(pad/8)); 
 
 buff = (unsigned char *)malloc(x*y*3);
 if( buff == NULL) 
   {
    fclose(f);
    return;
   }

 fread(buff,x*y*3,1,f);

 MapShell(out);


 t0 = time(0);

 for(i=0,ypos=0 ; ypos<y ; ypos++)
    for(xpos=0; xpos<x ; xpos++ )
       {
         AddPixel(xpos,ypos,buff[i+2],buff[i+1],buff[i]);
         i+=3;
        }

 t1 = time(0);

 fprintf(stderr,"Time=%d\n",t1-t0);
 XPutImage(gui.dpy,out->Win,out->gc,im,0,0,0,0,x,y);
 
 fclose(f);
 free(buff);
 WaitKey(out);
 DestroyShell(&gui,out);
 XDestroyImage(im);
}


void main()
{
 XColor tmp;
 short r, g, b, i, x, y;
 Colormap cmap;
 int mode;
 XWidget *entry;

 if( XConnect(&gui,"") == 0 )
   {
    printf("No Display\n");
    exit(0);
   }

 mode=DisplayPlanes(gui.dpy,DefaultScreen(gui.dpy));

 switch(mode)
       {
        case(24):
             AddPixel = &AddPixel24;
             printf("TRUE COLOR VISUAL: 24 Bits\n");
             break;
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


 sh = CreateShell(&gui,"TGA Viewer By C.LEGENDRE",0,0,230,40);

 MapShell(sh);
 entry = CreateButton(sh,"Load",10,10,70,20);
 AddCallBack(entry,TGAOpen);
 entry = CreateButton(sh,"Quit",140,10,70,20);
 AddCallBack(entry,Quit);
 MapAllWidget(sh);
 LoopShellEvent(&gui,sh);
 exit(0);
}

         
