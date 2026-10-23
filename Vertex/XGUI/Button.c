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

void SetIcon(XWidget *wid,char **data)
{
 XButtonWidget *wid2;
 Pixmap tmp;
 int ret;

 if( wid->button.Type == COMMAND )
   {
    wid2 = &(wid->button);
    if( XpmCreatePixmapFromData(wid2->dpy,wid2->Win,data,&tmp,NULL,NULL)==0 )
      {
       XSetForeground(wid2->dpy,wid2->gc,wid2->BOCOLOR);
       XFillRectangle(wid2->dpy,wid2->Fore,wid2->gc,0,0,wid2->Width,wid2->Height);
       XSetForeground(wid2->dpy,wid2->gc,wid2->SHCOLOR);
       XFillRectangle(wid2->dpy,wid2->Fore,wid2->gc,2,2,wid2->Width-4,wid2->Height-4);
       XSetForeground(wid2->dpy,wid2->gc,wid2->FGCOLOR);
       XFillRectangle(wid2->dpy,wid2->Fore,wid2->gc,3,3,wid2->Width-5,wid2->Height-5);
   
       XCopyArea(wid2->dpy,tmp,wid2->Fore,wid2->gc,0,0,wid2->Width,wid2->Height,2,2);
       XSetWindowBackgroundPixmap(wid2->dpy,wid2->Win,wid2->Fore);
       XCopyArea(wid2->dpy,wid2->Fore,wid2->Win,wid2->gc,0,0,wid2->Width,wid2->Height,0,0);
       XFreePixmap(wid2->dpy,tmp);
      }
  }
}


void ResizeButton(XButtonWidget *wid)
{
 int posx,posy;

 if( wid == NULL )
    return;

 if(wid->Fore != None)
    XFreePixmap(wid->dpy,wid->Fore);

 wid->Fore = XCreatePixmap(wid->dpy,wid->Win,
                                 wid->Width,wid->Height,
                                 DisplayPlanes(wid->dpy,DefaultScreen(wid->dpy)));

 posx = (wid->Width/2) - XTextWidth(wid->Font,wid->String,strlen(wid->String))/2;
 posy = (wid->Height/2) + (wid->Font->ascent+wid->Font->descent-3)/2;

 XSetForeground(wid->dpy,wid->gc,wid->BOCOLOR);
 XFillRectangle(wid->dpy,wid->Fore,wid->gc,0,0,wid->Width,wid->Height);
 XSetForeground(wid->dpy,wid->gc,wid->SHCOLOR);
 XFillRectangle(wid->dpy,wid->Fore,wid->gc,2,2,wid->Width-4,wid->Height-4);
 XSetForeground(wid->dpy,wid->gc,wid->FGCOLOR);
 XFillRectangle(wid->dpy,wid->Fore,wid->gc,3,3,wid->Width-5,wid->Height-5);
 if( strlen(wid->String) )
   {
    XSetForeground(wid->dpy,wid->gc,wid->BGCOLOR);
    XDrawString(wid->dpy,wid->Fore,wid->gc,posx,posy,wid->String,strlen(wid->String));
   }

 XSetWindowBackgroundPixmap(wid->dpy,wid->Win,wid->Fore);
} 


void PressButton(XButtonWidget *wid)
{
 XEvent ev;
 int bpress;

 XFlush(wid->dpy);
 if(wid->Type == REPEATER)
   {
     bpress = 1;
     while(bpress)
          {
           if(wid->CallBack)
              wid->CallBack();
           bpress = !XCheckTypedEvent(wid->dpy,ButtonRelease,&ev);
           usleep(150);
          }
   }
 else
    if(wid->CallBack)
       wid->CallBack();

}   

XWidget *CreateGButton(Display *dpy,Window win,const char *txt,int x,int y,int tx,int ty,int flag,unsigned long BG,unsigned long BO,unsigned long FG,unsigned long SH)
{
 XWidget *wid;  
 XGCValues gcv;

 wid = (XWidget *)calloc(sizeof(XButtonWidget),1);
 if(wid == NULL ) 
    return(NULL);

 if(dpy == NULL)
   {
    free(wid);
    return(NULL);
   }

 wid->button.dpy = dpy;
 wid->button.Resize = &ResizeButton;
 wid->button.Press = &PressButton;
 wid->button.Type = flag; 
 wid->button.CallBack = NULL;
 wid->button.Width = tx;
 wid->button.Height = ty;
 wid->button.BGCOLOR = BG;
 wid->button.FGCOLOR = FG;
 wid->button.SHCOLOR = SH;
 wid->button.BOCOLOR = BO;

 memset(wid->button.String,0,260);
 if( txt )
   {
    strncpy(&(wid->button.String[0]),txt,259);
    wid->button.String[259] = 0;
   }
 wid->button.Win = XCreateSimpleWindow(dpy,
                                       win,
                                       x, y,
                                       tx,ty,
                                       1,BG,FG);
 gcv.function = GXcopy;
 wid->button.Font = XGUIGetFont(dpy);
 if(wid->button.Font == NULL)
   {
    XDestroyWindow(dpy,wid->button.Win);
    free(wid);
    XFlush(dpy);
    return(NULL);
   }
 gcv.font = wid->button.Font->fid;

 wid->button.gc = XCreateGC(dpy,wid->button.Win,GCFunction|GCFont,&gcv);
 XSelectInput(dpy,wid->button.Win,ButtonPress|ButtonReleaseMask);

/*
 wid->button.Fore = XCreatePixmap(wid->button.dpy,wid->button.Win,
                                 wid->button.Width,wid->button.Height,
                                 DisplayPlanes(wid->button.dpy,DefaultScreen(wid->button.dpy)));
*/

 ResizeButton((XButtonWidget *)&(wid->button));
 return(wid);
}

XWidget *CreateButton(XShell *sh,const char *txt,int x,int y,int tx,int ty)
{
 XWidget *act;

 if( sh == NULL || sh->NextFree >= MAXWIDGET )
    return(NULL);

 act = CreateGButton(sh->dpy,sh->Win,txt,x,y,tx,ty,COMMAND,sh->BGCOLOR,sh->BOCOLOR,sh->FGCOLOR,sh->SHCOLOR);
 if(act != NULL)
    sh->List[sh->NextFree++]=act;
 return(act);
}

XWidget *CreateRepeater(XShell *sh,const char *txt,int x,int y,int tx,int ty)
{
 XWidget *act;

 if( sh == NULL || sh->NextFree >= MAXWIDGET )
    return(NULL);

 act = CreateGButton(sh->dpy,sh->Win,txt,x,y,tx,ty,REPEATER,sh->BGCOLOR,sh->BOCOLOR,sh->FGCOLOR,sh->SHCOLOR);
 if(act != NULL)
    sh->List[sh->NextFree++]=act;
 return(act);
}


