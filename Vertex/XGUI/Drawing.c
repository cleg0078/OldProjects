/********************************************************/
/*             Graphic User Interface Librairie         */
/*                         V1.2                         */
/*------------------------------------------------------*/
/* Concept And Design: C.LEGENDRE  April 1996           */
/*------------------------------------------------------*/
/* Freely use. Spread The Disk and Not The Word!!!      */
/* But Remember: This is My Job...                      */
/********************************************************/
#include"XGUI.h"


void ClearShell(XShell *sh)
{
 XClearWindow(sh->dpy,sh->Win);
}

void Background(XShell *sh,int nb)
{
 XSetBackground(sh->dpy,sh->gc,nb);
}

void Foreground(XShell *sh,int nb)
{
 XSetForeground(sh->dpy,sh->gc,nb);
}

void Putpixel(XShell *sh,int x,int y)
{
 Drawable target = (sh->BackBuffer != None) ? sh->BackBuffer : sh->Win;
 XDrawPoint(sh->dpy,target,sh->gc,x,y);
}

void GetPixel(XShell *sh,int x,int y,unsigned char *r,unsigned char *g,unsigned char *b)
{
 XImage *im;
 XColor col;
 Drawable src;

 if(r) *r = 0;
 if(g) *g = 0;
 if(b) *b = 0;
 if(sh == NULL || r == NULL || g == NULL || b == NULL)
    return;

 src = (sh->BackBuffer != None) ? sh->BackBuffer : sh->Win;
 im = XGetImage(sh->dpy,src,x,y,1,1,AllPlanes,ZPixmap);
 if(im == NULL)
    return;

 col.pixel = XGetPixel(im,0,0);
 XDestroyImage(im);

 if( XQueryColor(sh->dpy,DefaultColormap(sh->dpy,DefaultScreen(sh->dpy)),&col) )
    {
     *r = (unsigned char)(col.red   >> 8);
     *g = (unsigned char)(col.green >> 8);
     *b = (unsigned char)(col.blue  >> 8);
    }
}

void Line(XShell *sh,int x1,int y1,int x2,int y2)
{
 Drawable target = (sh->BackBuffer != None) ? sh->BackBuffer : sh->Win;
 XDrawLine(sh->dpy,target,sh->gc,x1,y1,x2,y2);
}

void Circle(XShell *sh,int x,int y,int r)
{
 Drawable target = (sh->BackBuffer != None) ? sh->BackBuffer : sh->Win;
 XDrawArc(sh->dpy,target,sh->gc,x,y,r,r,0,360*64);
}

void Fillcircle(XShell *sh,int x,int y,int r)
{
 Drawable target = (sh->BackBuffer != None) ? sh->BackBuffer : sh->Win;
 XFillArc(sh->dpy,target,sh->gc,x,y,r,r,0,360*64);
}

void Arc(XShell *sh,int x,int y,int r,int d,int e)
{
 Drawable target = (sh->BackBuffer != None) ? sh->BackBuffer : sh->Win;
 XDrawArc(sh->dpy,target,sh->gc,x,y,r,r,d*64,e*64);
}

void Fillarc(XShell *sh,int x,int y,int r,int d,int e)
{
 Drawable target = (sh->BackBuffer != None) ? sh->BackBuffer : sh->Win;
 XFillArc(sh->dpy,target,sh->gc,x,y,r,r,d*64,e*64);
}

void Rectangle(XShell *sh,int x,int y,int w,int h)
{
 Drawable target = (sh->BackBuffer != None) ? sh->BackBuffer : sh->Win;
 XDrawRectangle(sh->dpy,target,sh->gc,x,y,w,h);
}

void Fillrect(XShell *sh,int x,int y,int w,int h)
{
 Drawable target = (sh->BackBuffer != None) ? sh->BackBuffer : sh->Win;
 XFillRectangle(sh->dpy,target,sh->gc,x,y,w,h);
}

void SetLinewidth(XShell *sh,int w)
{
 XGCValues gcv;

 gcv.line_width = w;
 XChangeGC(sh->dpy,sh->gc,GCLineWidth,&gcv);
}
 

void SetDrawmode(XShell *sh,int mode)
{
  XGCValues gcv;

 /* Valid Mode: COPY,OR,AND,XOR... see XGUI.h */
 gcv.function = mode;
 XChangeGC(sh->dpy,sh->gc,GCFunction,&gcv);
}

void Text(XShell *sh,int x,int y,const char *msg)
{
 Drawable target = (sh->BackBuffer != None) ? sh->BackBuffer : sh->Win;
 XDrawString(sh->dpy,target,sh->gc,x,y,msg,strlen(msg));
}

int GetMouseXY(XShell *sh,int *x,int *y)
{
 XEvent ev;
 int cont;

 cont = XCheckWindowEvent(sh->dpy,sh->Win,PointerMotionMask,&ev);
 if( cont != False )
   {
    *x = ev.xbutton.x;
    *y = ev.xbutton.y;
    return(1);
   }
 
 return(-1);
}

char WaitKey(XShell *sh)
{
 char test[2];
 KeySym key;
 XEvent ev;
 int cont;

 cont=1;
 while(cont)
      {
        XNextEvent(sh->dpy,&ev);
        
        if(ev.type==KeyRelease || ev.type==KeyPress) 
          {
           XLookupString((XKeyEvent *)&ev,test,1,&key,NULL);
           cont = 0;
          }
      }

 return(test[0]);
}


char WaitEscape(XShell *sh)
{
 char test[2];
 KeySym key;
 XEvent ev;

 for(;;)
   {
    XNextEvent(sh->dpy,&ev);
    if(ev.type==KeyPress)
      {
       XLookupString((XKeyEvent *)&ev,test,1,&key,NULL);
       if(key == XK_Escape)
         return(1);
      }
   }
}

char GetKey(XShell *sh)
{
 char test[2];
 KeySym key;
 XEvent ev;
 int cont;

 test[0] = 0;
 cont = XCheckWindowEvent(sh->dpy,sh->Win,KeyReleaseMask,&ev);
 if( cont != False )
    XLookupString((XKeyEvent *)&ev,test,1,&key,NULL);       

 return(test[0]);
}


int PressMouseButton(XShell *sh,int *x,int *y)
{
 XEvent ev;
 int cont;

 cont = XCheckWindowEvent(sh->dpy,sh->Win,ButtonPressMask,&ev);
 if( cont != False )
   {
    *x = ev.xbutton.x;
    *y = ev.xbutton.y;
    return( ev.xbutton.button );
   }

 return(-1);
}


int ReleaseMouseButton(XShell *sh,int *x,int *y)
{
 XEvent ev;
 int cont;

 cont = XCheckWindowEvent(sh->dpy,sh->Win,ButtonReleaseMask,&ev);
 if( cont != False )
   {
    *x = ev.xbutton.x;
    *y = ev.xbutton.y;
    return( ev.xbutton.button );
   }

 return(-1);
}
