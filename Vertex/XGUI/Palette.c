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
#include"Connect.h"

extern XWidget *CreateGButton(Display *,Window,const char *,int,int,int,int,int,unsigned long,unsigned long,unsigned long,unsigned long);

XShell *psh;
XGUI *palette_intgui;
XWidget *r,*g,*b;

void DestroyPalette()
{
 DestroyShell(palette_intgui,psh);
 psh=NULL;
}

void DrawBox(int x,int y,int w,int h,int c) 
{
 XSetForeground(psh->dpy,psh->gc,psh->BOCOLOR);
 XFillRectangle(psh->dpy,psh->Win,psh->gc,x,y,w,h);
 XSetForeground(psh->dpy,psh->gc,c);
 XFillRectangle(psh->dpy,psh->Win,psh->gc,x+1,y+1,w-2,h-2);
}

void CreatePalette(XGUI *gui,const char *name,int px,int py)
{
 XWidget *ok,*cancel;
 XWidget *b;
 int col,x,y;

 palette_intgui = gui;

 psh = (XShell *)CreateShell(gui,name,px,py,320,280);
 ok = (XWidget *)CreateButton(psh,"Ok",10,250,80,20);
/* AddCallBack(ok,NULL);
*/
 cancel = (XWidget *)CreateButton(psh,"Cancel",230,250,80,20);
 AddCallBack(cancel,DestroyPalette);

 for(col=0,y=10 ; y<151 ; y+=42 )
     for(x=140 ; x<=280 ; x+=42 )
        {
         DrawBox(x,y,40,40,col++);
        }
 
 r = (XWidget *)CreateScroll(psh,20,10,10,140,1);
 g = (XWidget *)CreateScroll(psh,60,10,10,140,1);
 b = (XWidget *)CreateScroll(psh,100,10,10,140,1);
 
 MapShell(psh);
 MapAllWidget(psh);

 LoopShellEvent(palette_intgui,psh);
 return;
}
 
