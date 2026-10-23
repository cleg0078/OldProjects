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

extern XWidget *CreateGButton(Display *,Window,char *,int ,int ,int ,int, int,unsigned long ,unsigned long,unsigned long,unsigned long);

void ResizeMenu(XMenuWidget *wid)
{
 int posx,posy;

 if(wid == NULL)
    return;

 if(wid->Fore != None)
    XFreePixmap(wid->dpy,wid->Fore);

 wid->Fore = XCreatePixmap(wid->dpy,wid->Win,wid->Width,
                           wid->Height,DisplayPlanes(wid->dpy,DefaultScreen(wid->dpy)));

 posx = (wid->Width/2) - XTextWidth(wid->Font,wid->String,strlen(wid->String))/2;
 posy = (wid->Height/2) + (wid->Font->ascent+wid->Font->descent-3)/2;

 XSetForeground(wid->dpy,wid->gc,wid->BOCOLOR);
 XFillRectangle(wid->dpy,wid->Fore,wid->gc,0,0,wid->Width,wid->Height);
 XSetForeground(wid->dpy,wid->gc,wid->SHCOLOR);
 XFillRectangle(wid->dpy,wid->Fore,wid->gc,2,2,wid->Width-4,wid->Height-4);
 XSetForeground(wid->dpy,wid->gc,wid->FGCOLOR);
 XFillRectangle(wid->dpy,wid->Fore,wid->gc,3,3,wid->Width-5,wid->Height-5);
 XSetForeground(wid->dpy,wid->gc,wid->BGCOLOR);
 XDrawString(wid->dpy,wid->Fore,wid->gc,posx,posy,wid->String,strlen(wid->String));
 XSetWindowBackgroundPixmap(wid->dpy,wid->Win,wid->Fore);
 XFlush(wid->dpy);
} 

void PressMenu(XMenuWidget *wid)
{
 int i,cont=1;
 XEvent ev;

 if(wid == NULL)
    return;
 if(wid->NextFree == 0)
    return;

 XMapRaised(wid->dpy,wid->MenuWin); 
 XMapSubwindows(wid->dpy,wid->MenuWin);

 while(cont)
      {
       XNextEvent(wid->dpy,&ev);

       if( ev.type == ButtonPress )
          cont = 0;              
      }

 XUnmapWindow(wid->dpy,wid->MenuWin);

 for(i=0 ; i<wid->NextFree && ev.xany.window != wid->List[i]->Win ; i++);
 
 if( i<wid->NextFree && ev.xbutton.window == wid->List[i]->Win)
    if( wid->List[i]->CallBack)
       wid->List[i]->CallBack(); 
}

XWidget *CreateMenu(XShell *sh,char *txt,int x,int y,int tx,int ty)
{
 XWidget *wid;  
 Display *dpy;
 XGCValues gcv;

 wid = (XWidget *)calloc(sizeof(XMenuWidget),1);
 if(wid == NULL) 
    return(NULL);

 if( sh == NULL )
   {
    free(wid);
    return(NULL);
   }

 if( sh->NextFree >= MAXWIDGET )
   {
    free(wid);
    return(NULL);
   }

 if( txt == NULL )
    txt = "";

 if( (wid->menu.String = (char *)malloc(strlen(txt)+1)) == NULL )
   {
    free(wid);
    return(NULL);
   }

 dpy = wid->menu.dpy = sh->dpy;
 sh->List[sh->NextFree++] = wid;
 wid->menu.Resize = &ResizeMenu;
 wid->menu.Press = &PressMenu;
 wid->menu.CallBack = NULL;
 wid->menu.Type = MENU; 
 wid->menu.NextFree = 0;
 wid->menu.Width = tx;  wid->menu.Height = ty;
 wid->menu.Posx = x;    wid->menu.Posy = y;
 wid->menu.Maxx = tx;   wid->menu.Maxy = 0;
 wid->menu.BGCOLOR = sh->BGCOLOR;
 wid->menu.FGCOLOR = sh->FGCOLOR;
 wid->menu.SHCOLOR = sh->SHCOLOR;
 wid->menu.BOCOLOR = sh->BOCOLOR;
 strcpy(wid->menu.String,txt);

 wid->menu.Win = XCreateSimpleWindow(dpy,
                                     sh->Win,
                                     x, y,
                                     tx,ty,
                                     1,sh->BGCOLOR,sh->FGCOLOR);

 wid->menu.MenuWin = XCreateSimpleWindow(dpy,
                                         sh->Win,
                                         x,y+ty+2,
                                         2*tx,ty,
                                         2,sh->BOCOLOR,sh->FGCOLOR);
                                       
 gcv.function = GXcopy;
 wid->menu.Font = XGUIGetFont(dpy);
 if(wid->menu.Font == NULL)
   {
    XDestroyWindow(dpy,wid->menu.MenuWin);
    XDestroyWindow(dpy,wid->menu.Win);
    free(wid->menu.String);
    sh->List[--sh->NextFree] = NULL;
    free(wid);
    XFlush(dpy);
    return(NULL);
   }
 gcv.font = wid->menu.Font->fid;
 wid->menu.gc = XCreateGC(dpy,wid->menu.Win,GCFunction|GCFont,&gcv);
 ResizeMenu((XMenuWidget *)&(wid->menu));
 XSelectInput(dpy,wid->menu.Win,ButtonPress|ButtonReleaseMask);
 XSelectInput(dpy,wid->menu.MenuWin,ButtonPress|ButtonReleaseMask);
 XFlush(dpy);
 return(wid);
}


XWidget *AddMenuItem(XWidget *wid,char *txt)
{
 XWidget *act;
 int i,tx,posy,ty;
 if(wid == NULL || wid->any.Type != MENU )
    return(NULL);

 if( wid->menu.NextFree >= 20 )
    return(NULL);

 ty = wid->menu.Font->ascent+wid->menu.Font->descent+6;
 posy = (wid->menu.NextFree)*ty;
 wid->menu.Maxy +=ty;

 tx = XTextWidth(wid->menu.Font,txt,strlen(txt))+10;
 if( wid->menu.Maxx < tx ) 
   {
     wid->menu.Maxx = tx;
     for(i=0; i<wid->menu.NextFree ; i++)
        {
         XResizeWindow(wid->menu.dpy,wid->menu.List[i]->Win,wid->menu.Maxx,ty);
         wid->menu.List[i]->Width = wid->menu.Maxx;
         wid->menu.List[i]->Resize(wid->menu.List[i]); 
        }
   }

 act = CreateGButton(wid->menu.dpy,wid->menu.MenuWin,txt,0,posy,wid->menu.Maxx,ty,COMMAND,wid->menu.BGCOLOR,wid->menu.BOCOLOR,wid->menu.FGCOLOR,wid->menu.SHCOLOR);

 if(act != NULL)
   {
      wid->menu.List[wid->menu.NextFree++] = (XButtonWidget *)act;
      XResizeWindow(wid->menu.dpy,wid->menu.MenuWin,wid->menu.Maxx+2,wid->menu.Maxy+2);
   }

 return(act);
}

