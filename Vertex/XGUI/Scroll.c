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

XScrollWidget *XCurrentScroll = NULL;  /* Scroll widget reporting to its CallBack */

int ScrollRange(XScrollWidget *wid)
{
 int dpl,range;

 dpl = (wid->Orientation==VSCROLL)?wid->Height:wid->Width;
 range = dpl - dpl/8 - 2;
 if( range < 1 )
    range = 1;
 return(range);
}

static void ClampAct(XScrollWidget *wid)
{
 int range = ScrollRange(wid);

 if( wid->Act < 0 )
    wid->Act = 0;
 if( wid->Act > range )
    wid->Act = range;
}

static void MoveThumb(XScrollWidget *wid)
{
 if( wid->Orientation == VSCROLL )
    XMoveWindow(wid->dpy,wid->Win,0,wid->Act);
 else
    XMoveWindow(wid->dpy,wid->Win,wid->Act,0);
}

static void UpdatePos(XScrollWidget *wid)
{
 float f = (float)wid->Act / (float)ScrollRange(wid);

 wid->Pos = wid->Min + f * (wid->Max - wid->Min);
 if( wid->Pos < wid->Min )
    wid->Pos = wid->Min;
 if( wid->Pos > wid->Max )
    wid->Pos = wid->Max;
}

static int ActFromPos(XScrollWidget *wid,float pos)
{
 float f = 0.0;

 if( wid->Max > wid->Min )
    f = (pos - wid->Min) / (wid->Max - wid->Min);
 if( f < 0.0 )
    f = 0.0;
 if( f > 1.0 )
    f = 1.0;
 return((int)(f * ScrollRange(wid) + 0.5f));
}

static void ScrollNotify(XScrollWidget *wid)
{
 XCurrentScroll = wid;
 if( wid->CallBack )
    wid->CallBack();
}

void ScrollStep(XScrollWidget *wid,int dir)
{
 int step;

 if( wid == NULL || wid->Type != SCROLLBAR )
    return;

 step = ScrollRange(wid)/10;
 if( step < 1 )
    step = 1;

 wid->Act += dir*step;
 ClampAct(wid);
 MoveThumb(wid);
 UpdatePos(wid);
 ScrollNotify(wid);
 XFlush(wid->dpy);
}

void PressScroll(XScrollWidget *wid)
{
 XEvent ev;
 int cont=1,grabbed=0,begin=1,reported,dpl,thumb;
 Time ts;

 if( wid == NULL || wid->Type != SCROLLBAR )
    return;

 dpl = (wid->Orientation==VSCROLL)?wid->Height:wid->Width;
 thumb = dpl/8;
 if( thumb < 1 )
    thumb = 1;
 reported = wid->Act;

 XFlush(wid->dpy);
 if( XGrabPointer(wid->dpy,wid->ScrollWin,False,
                  ButtonPressMask|ButtonReleaseMask|PointerMotionMask,
                  GrabModeAsync,GrabModeAsync,None,None,CurrentTime) == GrabSuccess )
    grabbed = 1;

 ts = CurrentTime;

 while( cont )
      {
       XNextEvent(wid->dpy,&ev);

       if( !grabbed && ev.type != ButtonRelease &&
           ev.xany.window != wid->Win && ev.xany.window != wid->ScrollWin )
         {
          XPutBackEvent(wid->dpy,&ev);   /* not ours: hand it back */
          break;                          /* grab-less fallback: end drag */
         }

       if( ev.type == ButtonPress || ev.type == ButtonRelease ||
           ev.type == MotionNotify )
          ts = ev.xbutton.time;

       switch(ev.type)
             {
              case(ButtonPress):
                   if( ev.xbutton.button == 4 || ev.xbutton.button == 5 )
                     {
                      ScrollStep(wid,(ev.xbutton.button==4)?-1:1);
                      reported = wid->Act;
                      break;
                     }
                   if( ev.xbutton.button != Button1 )
                      break;
                   if( !grabbed && ev.xany.window != wid->ScrollWin )
                      break;
                   {
                    int click = (wid->Orientation==VSCROLL)?ev.xbutton.y:ev.xbutton.x;
                    if( click < wid->Act )
                       wid->Act -= thumb;
                    else
                       if( click > wid->Act + thumb )
                          wid->Act += thumb;
                       else
                          break;   /* Click on the thumb itself: drag only */
                    ClampAct(wid);
                    MoveThumb(wid);
                    begin = 1;
                   }
                   break;

              case(MotionNotify):
                   if( (ev.xmotion.state & Button1Mask) == 0 )
                      break;
                   {
                    int btroot = (wid->Orientation==VSCROLL)?ev.xmotion.y_root:ev.xmotion.x_root;
                    if( begin == 1 )
                      {
                       begin = 0;
                       wid->ActRoot = btroot;
                      }
                    else
                      {
                       wid->Act += btroot - wid->ActRoot;
                       wid->ActRoot = btroot;
                       ClampAct(wid);
                       MoveThumb(wid);
                      }
                   }
                   break;

              case(ButtonRelease):
                   if( ev.xbutton.button == 4 || ev.xbutton.button == 5 )
                      break;
                   cont = 0;
                   break;
             }

       if( wid->Act != reported )
          {
           reported = wid->Act;
           UpdatePos(wid);
           ScrollNotify(wid);
          }
      }

 UpdatePos(wid);

 if( grabbed )
    XUngrabPointer(wid->dpy,ts);

 XFlush(wid->dpy);
}


XWidget *CreateVHScroll(XShell *sh,Display *dpy,Window win,int x,int y,int tx,int ty,int orient)
{
 XWidget *wid;  
 XGCValues gcv;

 wid = (XWidget *)calloc(sizeof(XScrollWidget),1);
 if(wid == NULL) 
    return(NULL);

 if( dpy == NULL )
   {
    free(wid);
    return(NULL);
   }

 wid->scroll.dpy = dpy;
 wid->scroll.Press = &PressScroll;
 wid->scroll.Type = SCROLLBAR; 
 wid->scroll.Pos = 0.0;
 wid->scroll.Min = 0.0;    wid->scroll.Max = 100.0;
 wid->scroll.Width = tx;   wid->scroll.Height = ty;
 wid->scroll.Orientation = orient;
 wid->scroll.Act = 0;
 wid->scroll.ActRoot = (orient==VSCROLL)?y:x;
 wid->scroll.CallBack = NULL;
 wid->scroll.BGCOLOR = sh->BGCOLOR;
 wid->scroll.FGCOLOR = sh->FGCOLOR;
 wid->scroll.SHCOLOR = sh->SHCOLOR;
 wid->scroll.BOCOLOR = sh->BOCOLOR;

 wid->scroll.ScrollWin = XCreateSimpleWindow(dpy,
                                             win,
                                             x,y,
                                             tx,ty,
                                             1,wid->scroll.BGCOLOR,wid->scroll.FGCOLOR);

 wid->scroll.Win = XCreateSimpleWindow(dpy,
                                       wid->scroll.ScrollWin,
                                       0, 0,
                                       (orient==VSCROLL)?tx-2:tx>>3,(orient==VSCROLL)?ty>>3:ty-2,
                                       1,wid->scroll.BGCOLOR,wid->scroll.FGCOLOR);

 gcv.function = GXcopy;
 wid->scroll.gc = XCreateGC(dpy,wid->scroll.Win,GCFunction,&gcv);
 
 wid->scroll.Fore = XCreatePixmap(wid->scroll.dpy,wid->scroll.ScrollWin,wid->scroll.Width,wid->scroll.Height,DisplayPlanes(wid->scroll.dpy,DefaultScreen(wid->scroll.dpy)));
 

 XClearWindow(wid->scroll.dpy,wid->scroll.Win);
 XSetForeground(wid->scroll.dpy,wid->scroll.gc,wid->scroll.BOCOLOR);
 XFillRectangle(wid->scroll.dpy,wid->scroll.Fore,wid->scroll.gc,0,0,wid->scroll.Width,wid->scroll.Height);
 XSetForeground(wid->scroll.dpy,wid->scroll.gc,wid->scroll.SHCOLOR);
 XFillRectangle(wid->scroll.dpy,wid->scroll.Fore,wid->scroll.gc,2,2,wid->scroll.Width-4,wid->scroll.Height-4);
 XSetForeground(wid->scroll.dpy,wid->scroll.gc,wid->scroll.FGCOLOR);
 XFillRectangle(wid->scroll.dpy,wid->scroll.Fore,wid->scroll.gc,3,3,wid->scroll.Width-5,wid->scroll.Height-5);
 XSetWindowBackgroundPixmap(wid->scroll.dpy,wid->scroll.ScrollWin,wid->scroll.Fore);
 XSetWindowBackgroundPixmap(wid->scroll.dpy,wid->scroll.Win,wid->scroll.Fore);

 XSelectInput(dpy,wid->scroll.ScrollWin,ButtonPressMask|ButtonReleaseMask|PointerMotionMask);
 XSelectInput(dpy,wid->scroll.Win,ButtonPressMask|ButtonReleaseMask|PointerMotionMask);
 
 XFlush(dpy);
 return(wid);
}


XWidget *CreateScroll(XShell *sh,int x,int y,int tx,int ty,int o)
{
 XWidget *act;

 if( sh == NULL || sh->NextFree >= MAXWIDGET )
    return(NULL);

 switch(o)
       {
        case(VSCROLL):
        case(HSCROLL):
            act = CreateVHScroll(sh,sh->dpy,sh->Win,x,y,tx,ty,o);
            break;
        default:
            act = CreateVHScroll(sh,sh->dpy,sh->Win,x,y,tx,ty,VSCROLL); 
       }
 
 if( act != NULL)
    sh->List[sh->NextFree++] = act;
  
 return(act);
} 

void GetScrollPos(XWidget *wid,float *pos)
{
 if( (wid != NULL) && (wid->any.Type == SCROLLBAR) )
    *pos = wid->scroll.Pos;
}

void SetScrollPos(XWidget *wid,float pos)
{
 XScrollWidget *wid1;

 if( wid == NULL || wid->any.Type != SCROLLBAR )
    return;

 wid1 = &(wid->scroll);

 if( pos < wid1->Min )
    pos = wid1->Min;
 if( pos > wid1->Max )
    pos = wid1->Max;

 wid1->Pos = pos;
 wid1->Act = ActFromPos(wid1,pos);
 MoveThumb(wid1);
 XFlush(wid1->dpy);
}

