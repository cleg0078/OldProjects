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

void ResizeCheckBox(XCheckBoxWidget *wid)
{
 XGCValues gcv;
 int posx,posy;
 int debrect;

 if(wid == 0 )
    return;

 switch(wid->TextPlace)
       {
        case(LEFT): 
             posx = wid->Px - XTextWidth(wid->Font,wid->String,strlen(wid->String))-XTextWidth(wid->Font," ",1)/2;
             posy = wid->Py + (wid->Height/2)+(wid->Font->ascent+wid->Font->descent-3)/2;
             break;
        case(RIGHT):
             posx = wid->Px + wid->Width + XTextWidth(wid->Font," ",1);
             posy = wid->Py + (wid->Height/2)+(wid->Font->ascent+wid->Font->descent-3)/2;
             break;
        case(ABOVE):
             posx = wid->Px +(wid->Width/2) - XTextWidth(wid->Font,wid->String,strlen(wid->String))/2;
             posy = wid->Py  - (wid->Font->ascent+wid->Font->descent-3)/2;
             break;
        case(BELOW):
             posx = wid->Px +(wid->Width/2) - XTextWidth(wid->Font,wid->String,strlen(wid->String))/2;
             posy = wid->Py + wid->Height + 3*(wid->Font->ascent+wid->Font->descent-3)/2;  
             break;
        default:   /* CENTER (and anything else): label centered over the box.
                      posx/posy were left uninitialized before this fix. */
             posx = wid->Px +(wid->Width/2) - XTextWidth(wid->Font,wid->String,strlen(wid->String))/2;
             posy = wid->Py + (wid->Height/2)+(wid->Font->ascent+wid->Font->descent-3)/2;
             break;
      }
              
 XSetForeground(wid->dpy,wid->gc,wid->BOCOLOR);
 XFillRectangle(wid->dpy,wid->Fore,wid->gc,0,0,wid->Width,wid->Height);

 XSetForeground(wid->dpy,wid->gc,wid->SHCOLOR);
 XFillRectangle(wid->dpy,wid->Fore,wid->gc,2,2,wid->Width-4,wid->Height-4);

 XSetForeground(wid->dpy,wid->gc,wid->FGCOLOR);
 XFillRectangle(wid->dpy,wid->Fore,wid->gc,3,3,wid->Width-5,wid->Height-5);

 if( wid->State == ACTIVE )
   {
    XSetForeground(wid->dpy,wid->gc,wid->BOCOLOR);

    if( wid->Width >= 20 && wid->Height>=20 )
      {
       gcv.line_width = 2;
       XChangeGC(wid->dpy,wid->gc,GCLineWidth,&gcv);
      }

    XDrawLine(wid->dpy,wid->Fore,wid->gc,wid->Width/5,wid->Height/5,2*wid->Width/5,4*wid->Height/5);
    XDrawLine(wid->dpy,wid->Fore,wid->gc,2*wid->Width/5,4*wid->Height/5,3*wid->Width/5,wid->Height/3);
    XDrawLine(wid->dpy,wid->Fore,wid->gc,3*wid->Width/5,wid->Height/3,4*wid->Width/5,wid->Height/3);
    gcv.line_width = 1;
    XChangeGC(wid->dpy,wid->gc,GCLineWidth,&gcv);
   }

 
 XDrawString(wid->dpy,wid->parent,wid->gc,posx,posy,wid->String,strlen(wid->String));

 XSetWindowBackgroundPixmap(wid->dpy,wid->Win,wid->Fore);
 XCopyArea(wid->dpy,wid->Fore,wid->Win,wid->gc,0,0,wid->Width,wid->Height,0,0);
} 

void PressCheck(XCheckBoxWidget *wid)
{
 wid->State = (wid->State == NOTACTIVE)?ACTIVE:NOTACTIVE; 
 ResizeCheckBox(wid);
 if( wid->CallBack )
    wid->CallBack();
}

XWidget *CreateCheckBox(XShell *sh,const char *txt,int x,int y,int tx,int ty,int flag)
{
 XWidget *wid;  
 XGCValues gcv;

 wid = (XWidget *)calloc(sizeof(XCheckBoxWidget),1);
 if( wid == NULL )
    return(NULL);

 if( sh == NULL || sh->dpy == NULL || sh->NextFree >= MAXWIDGET )
   {
    free(wid);
    return(NULL);
   }

 wid->check.dpy= sh->dpy;
 wid->check.Resize = &ResizeCheckBox;
 wid->check.Press = &PressCheck;
 wid->check.Type = CHECKBOX;
 wid->check.State = NOTACTIVE;
 wid->check.Width = tx;
 wid->check.Height = ty;
 wid->check.TextPlace = flag;
 wid->check.Px = x;
 wid->check.Py = y;
 wid->check.parent = sh->Win;
 wid->check.BGCOLOR = sh->BGCOLOR;
 wid->check.FGCOLOR = sh->FGCOLOR;
 wid->check.SHCOLOR = sh->SHCOLOR;
 wid->check.BOCOLOR = sh->BOCOLOR;
 wid->check.CallBack = NULL;

 memset(wid->check.String,0,260);
 strncpy(wid->check.String,txt,259);
 wid->check.String[259] = 0;

 wid->check.Win = XCreateSimpleWindow(sh->dpy,
                                      sh->Win,
                                      x, y,
                                      tx,ty,
                                      1,sh->BGCOLOR,sh->FGCOLOR);

 gcv.function = GXcopy;
 wid->check.Font = XGUIGetFont(sh->dpy);
 if(wid->check.Font == NULL)
   {
    XDestroyWindow(sh->dpy,wid->check.Win);
    free(wid);
    XFlush(sh->dpy);
    return(NULL);
   }
 gcv.font = wid->check.Font->fid;

 wid->check.gc = XCreateGC(sh->dpy,wid->check.Win,GCFunction|GCFont,&gcv);
 XSelectInput(sh->dpy,wid->check.Win,ButtonPress|ButtonReleaseMask);

 wid->check.Fore = XCreatePixmap(wid->check.dpy,wid->check.Win,
                                  wid->check.Width,wid->check.Height,
                                  DisplayPlanes(wid->check.dpy,
                                  DefaultScreen(wid->check.dpy)));

 ResizeCheckBox((XCheckBoxWidget *)&(wid->check));
 sh->List[sh->NextFree++]=wid;
 XFlush(sh->dpy);
 return(wid);
}

void ActiveCheckBox(XWidget *wid)
{
 if( wid->any.Type == CHECKBOX )
   {
    wid->check.State = NOTACTIVE;
    PressCheck(&(wid->check));
   }
}

void UnActiveCheckBox(XWidget *wid)
{
 if( wid->any.Type == CHECKBOX )
   {
    wid->check.State = ACTIVE;
    PressCheck(&(wid->check));
   }
}


int GetCheckBoxState(XWidget *wid)
{
  if( wid->any.Type == CHECKBOX )
    return( wid->check.State );
  else
    return(-2);
}

