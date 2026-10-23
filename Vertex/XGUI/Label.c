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

XWidget *CreateLabel(XShell *sh,char *txt,int x,int y,int fcolor,int bcolor)
{
 XWidget *wid;  
 Display *dpy;
 XGCValues gcv;
 int posx,posy;

 wid = (XWidget *)calloc(sizeof(XLabelWidget),1);
 if(sh==NULL || wid == NULL || sh->NextFree>=MAXWIDGET) 
    return(NULL);


 dpy = wid->menu.dpy = sh->dpy;
 sh->List[sh->NextFree++] = wid;
 wid->label.Type = LABEL; 
 wid->label.Font = XGUIGetFont(dpy);
 if(wid->label.Font == NULL)
   {
    sh->List[--sh->NextFree] = NULL;
    free(wid);
    XFlush(dpy);
    return(NULL);
   }
 wid->label.Width = XTextWidth(wid->label.Font,txt,strlen(txt))+10;
 wid->label.Height = wid->label.Font->ascent+wid->label.Font->descent+8; 
 wid->label.BGCOLOR = sh->BGCOLOR;
 wid->label.FGCOLOR = sh->FGCOLOR;
 wid->label.SHCOLOR = sh->SHCOLOR;
 wid->label.BOCOLOR = sh->BOCOLOR;

 strncpy(wid->label.String,txt,259);
 wid->label.String[259] = 0;

 wid->label.Win = XCreateSimpleWindow(dpy,
                                     sh->Win,
                                     x, y,
                                     wid->label.Width,wid->label.Height,
                                     0,sh->SHCOLOR,sh->SHCOLOR);
 wid->label.Fore = XCreatePixmap(dpy,wid->label.Win,wid->label.Width,wid->label.Height,DisplayPlanes(dpy,DefaultScreen(dpy)));
 gcv.function = GXcopy;
 gcv.font = wid->label.Font->fid;
 wid->label.gc = XCreateGC(dpy,wid->label.Win,GCFunction|GCFont,&gcv);

 XSetForeground(dpy,wid->label.gc,sh->SHCOLOR);
 XFillRectangle(dpy,wid->label.Fore,wid->label.gc,0,0,wid->label.Width,wid->label.Height);
 XSetForeground(dpy,wid->label.gc,fcolor);

 posx = (wid->label.Width/2) - XTextWidth(wid->label.Font,txt,strlen(txt))/2;
 if( posx < 0 )
    posx = 0;

 posy = (wid->label.Height/2) + (wid->label.Font->ascent+wid->label.Font->descent-3)/2;
 XDrawString(dpy,wid->label.Fore,wid->label.gc,posx,posy,txt,strlen(txt));  
 XSetWindowBackgroundPixmap(dpy,wid->label.Win,wid->label.Fore);
 XFlush(dpy);
 return(wid);
}

