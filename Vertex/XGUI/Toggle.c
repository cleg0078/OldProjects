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

void ResizeToggle(XToggleWidget *wid)
{
 int posx,posy;
 int debrect;

 if(wid == 0 )
    return;

 switch(wid->TextPlace)
       {
        case(LEFT): 
                    posx=0;   
                    break;
        default:   
                    posx = (wid->Width/2) - XTextWidth(wid->Font,wid->String,strlen(wid->String))/2;
      }
              
 posy = (wid->Height/2) + (wid->Font->ascent+wid->Font->descent-3)/2;


 if(wid->WBorder)
    {
     XSetForeground(wid->dpy,wid->gc,wid->BOCOLOR);
     XFillRectangle(wid->dpy,wid->Fore,wid->gc,0,0,wid->Width,wid->Height);
     debrect = 2;
    }
 else
    debrect = 0; 

 if(wid->State == ACTIVE )
    XSetForeground(wid->dpy,wid->gc,wid->BGCOLOR);
 else
    XSetForeground(wid->dpy,wid->gc,wid->SHCOLOR);

 if( wid->WBorder )
   XFillRectangle(wid->dpy,wid->Fore,wid->gc,debrect,debrect,wid->Width-4,wid->Height-4); 
 else
   XFillRectangle(wid->dpy,wid->Fore,wid->gc,debrect,debrect,wid->Width,wid->Height);

 if(wid->State == ACTIVE )
    XSetForeground(wid->dpy,wid->gc,wid->BGCOLOR);
 else
    XSetForeground(wid->dpy,wid->gc,wid->FGCOLOR);
    
 if( wid->WBorder )
   XFillRectangle(wid->dpy,wid->Fore,wid->gc,debrect+1,debrect+1,wid->Width-5,wid->Height-5);
 else
   XFillRectangle(wid->dpy,wid->Fore,wid->gc,debrect,debrect,wid->Width,wid->Height);

 if(wid->State == ACTIVE )
    XSetForeground(wid->dpy,wid->gc,wid->BOCOLOR);
 else
    XSetForeground(wid->dpy,wid->gc,wid->BGCOLOR);

 XDrawString(wid->dpy,wid->Fore,wid->gc,posx,posy,wid->String,strlen(wid->String));
 XSetWindowBackgroundPixmap(wid->dpy,wid->Win,wid->Fore);
 XCopyArea(wid->dpy,wid->Fore,wid->Win,wid->gc,0,0,wid->Width,wid->Height,0,0);
} 

void PressToggle(XToggleWidget *wid)
{
 wid->State = (wid->State == NOTACTIVE)?ACTIVE:NOTACTIVE; 
 ResizeToggle(wid);
 if( wid->CallBack )
    wid->CallBack();
}


void PressToggleWC(XToggleWidget *wid)  /* Without CallBack */
{
 wid->State = (wid->State == NOTACTIVE)?ACTIVE:NOTACTIVE;
 ResizeToggle(wid);
}

XWidget *CreateGToggle(Display *dpy,Window par,char *txt,int x,int y,int tx,int ty,int flag,int pl,unsigned long BG,unsigned long FG,unsigned long BO,unsigned long SH)
{
 XWidget *wid;  
 XGCValues gcv;

 wid = (XWidget *)calloc(sizeof(XToggleWidget),1);
 if(wid == NULL || dpy == NULL) 
    return(NULL);

 wid->toggle.dpy= dpy;
 wid->toggle.Resize = &ResizeToggle;
 wid->toggle.Press = &PressToggle;
 wid->toggle.Type = TOGGLE;
 wid->toggle.State = NOTACTIVE;
 wid->toggle.WBorder = flag;
 wid->toggle.Width = tx;
 wid->toggle.Height = ty;
 wid->toggle.TextPlace = pl;
 wid->toggle.BGCOLOR = BG;
 wid->toggle.FGCOLOR = FG;
 wid->toggle.SHCOLOR = SH;
 wid->toggle.BOCOLOR = BO;
 wid->toggle.CallBack = NULL;

 memset(wid->toggle.String,0,260);
 strncpy(wid->toggle.String,txt,259);
 wid->toggle.String[259] = 0;

 wid->toggle.Font = XGUIGetFont(dpy);
 if(wid->toggle.Font == NULL)
   {
    free(wid);
    XFlush(dpy);
    return(NULL);
   }

 wid->toggle.Win = XCreateSimpleWindow(dpy,
                                       par,
                                       x, y,
                                       tx,ty,
                                       flag,BG,FG);

 gcv.function = GXcopy;
 gcv.font = wid->toggle.Font->fid;

 wid->toggle.gc = XCreateGC(dpy,wid->toggle.Win,GCFunction|GCFont,&gcv);
 XSelectInput(dpy,wid->toggle.Win,ButtonPress|ButtonReleaseMask);

 wid->toggle.Fore = XCreatePixmap(wid->toggle.dpy,wid->toggle.Win,
                                  wid->toggle.Width,wid->toggle.Height,
                                  DisplayPlanes(wid->toggle.dpy,
                                  DefaultScreen(wid->toggle.dpy)));

 ResizeToggle((XToggleWidget *)&(wid->toggle));
 XFlush(dpy);
 return(wid);
}

XWidget *CreateToggle(XShell *sh,char *txt,int x,int y,int tx,int ty)
{
 XWidget *act;

 if( sh == NULL || sh->NextFree >= MAXWIDGET )
    return(NULL);

 act = CreateGToggle(sh->dpy,sh->Win,txt,x,y,tx,ty,1,CENTER,sh->BGCOLOR,sh->FGCOLOR,sh->BOCOLOR,sh->SHCOLOR);
 
 if(act != NULL)
   sh->List[sh->NextFree++] = act;

 return(act);
}

void ActiveToggle(XWidget *wid)
{
 if( wid->any.Type == TOGGLE )
   {
    wid->toggle.State = NOTACTIVE;
    PressToggleWC(&(wid->toggle));
   }
}

void UnActiveToggle(XWidget *wid)
{
 if( wid->any.Type == TOGGLE )
   {
    wid->toggle.State = ACTIVE;
    PressToggleWC(&(wid->toggle));
   }
}


int GetToggleState(XWidget *wid)
{
  if( wid->any.Type == TOGGLE )
    return( wid->toggle.State );
  else
    return(-2);
}

