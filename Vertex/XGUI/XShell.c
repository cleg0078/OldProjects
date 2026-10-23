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

XShell *CreateShell(XGUI *main,const char *title,int x,int y,int tx,int ty)
{
 XColor col1,col2;
 XShell *sh;
 XSizeHints shv;
 XGCValues gcv;
 XSetWindowAttributes attrs;
 int i;

 if( main == NULL)
    return(NULL);

 sh = (XShell *)calloc(1,sizeof(XShell));
 if( sh == NULL )
    return(NULL);
 
 sh->dpy = main->dpy;
 for(i=0;i<20 && main->List[i] != NULL ; i++);
 
 if(i<20)
    main->List[i] = sh;
 else
    {
     free(sh);
     return(NULL);
    }

 /* Our Good Color */
 sh->BOCOLOR = BlackPixel(sh->dpy,DefaultScreen(sh->dpy));
 sh->BGCOLOR = WhitePixel(sh->dpy,DefaultScreen(sh->dpy));

 XLookupColor(sh->dpy,DefaultColormap(sh->dpy,DefaultScreen(sh->dpy)),"#666666",&col1,&col2);
 col2.red=col2.green=col2.blue=0x66*256; 
 col2.flags=DoRed|DoGreen|DoBlue;
 XAllocColor(sh->dpy,DefaultColormap(sh->dpy,DefaultScreen(sh->dpy)),&col2);
 sh->FGCOLOR = col2.pixel;

 col2.red=col2.green=col2.blue=0xbb*256;
 col2.flags=DoRed|DoGreen|DoBlue;
 XAllocColor(sh->dpy,DefaultColormap(sh->dpy,DefaultScreen(sh->dpy)),&col2);
 sh->SHCOLOR = col2.pixel;

 sh->Win = XCreateSimpleWindow(sh->dpy,
                               DefaultRootWindow(sh->dpy),
                               x, y,
                               tx,ty,
                               1,sh->BGCOLOR,sh->SHCOLOR);

 shv.flags = PSize | PMinSize | PMaxSize;
 shv.width = shv.min_width = shv.max_width = tx;
 shv.height = shv.min_height = shv.max_height = ty;
 XSetStandardProperties( sh->dpy, sh->Win, title, title, None, NULL, 0, &shv );

 attrs.backing_store = Always;
 XChangeWindowAttributes(sh->dpy,sh->Win,CWBackingStore,&attrs);

 gcv.function = GXcopy;
 gcv.line_width = 1;
 {
  XFontStruct *shfont = XGUIGetFont(sh->dpy);
  gcv.font = (shfont != NULL) ? shfont->fid : XLoadFont(sh->dpy,DFONT);
 }
 sh->gc = XCreateGC(sh->dpy,sh->Win,GCFont|GCFunction|GCLineWidth,&gcv);

 XStoreName(sh->dpy,sh->Win,title);
 XSelectInput(sh->dpy,sh->Win,ButtonPressMask|ButtonReleaseMask|KeyReleaseMask|
                              KeyPressMask|PointerMotionMask|ExposureMask);
 
 /* Initialize back buffer to None */
 sh->BackBuffer = None;
 sh->Width = tx;
 sh->Height = ty;
 
 XFlush(sh->dpy);
 return(sh);
}  


/* Enable double buffering for a shell - call before drawing */
void EnableDoubleBuffer(XShell *sh)
{
 if(sh == NULL || sh->BackBuffer != None)
    return;
    
 sh->BackBuffer = XCreatePixmap(sh->dpy, sh->Win,
                                sh->Width, sh->Height,
                                DefaultDepth(sh->dpy, DefaultScreen(sh->dpy)));
 
 /* Clear the back buffer with the shell color */
 XSetForeground(sh->dpy, sh->gc, sh->SHCOLOR);
 XFillRectangle(sh->dpy, sh->BackBuffer, sh->gc, 0, 0, sh->Width, sh->Height);
}


/* Flip back buffer to window - call after drawing is complete */
void FlipBuffer(XShell *sh)
{
 if(sh == NULL || sh->BackBuffer == None)
    return;
    
 /* Copy pixmap to window */
 XCopyArea(sh->dpy, sh->BackBuffer, sh->Win, sh->gc,
           0, 0, sh->Width, sh->Height, 0, 0);
 
 /* Set pixmap as window background so it persists through Expose events */
 XSetWindowBackgroundPixmap(sh->dpy, sh->Win, sh->BackBuffer);
 
 XFlush(sh->dpy);
}


void DestroyBRTL(XWidget *wid)
{
  if(wid != NULL)
    {
     switch(wid->any.Type)
          {
           case(COMMAND):
           case(REPEATER):
           case(TOGGLE):
           case(CHECKBOX):
           case(LABEL):
                /* Font is the shared XGUIGetFont() cache: not owned, not freed */
                XFreePixmap(wid->any.dpy,wid->any.Fore);
                XFreeGC(wid->any.dpy,wid->any.gc);
                XDestroyWindow(wid->any.dpy,wid->any.Win);
                free(wid);
                break;
           default:      
          }
   }
}
         
void DestroySB(XScrollWidget *wid)
{
 if(wid != NULL && wid->Type == SCROLLBAR)
   {
    XFreePixmap(wid->dpy,wid->Fore);
    XFreeGC(wid->dpy,wid->gc);
    XDestroyWindow(wid->dpy,wid->Win);
    if(wid->ScrollWin != None)
       XDestroyWindow(wid->dpy,wid->ScrollWin);
    free(wid);
   }
}
 
void DestroyXWidget(XWidget *wid)
{
 int i;

 if(wid != NULL)
   {
    switch(wid->any.Type)
          {
           case(COMMAND):
           case(REPEATER):
           case(TOGGLE):
           case(CHECKBOX):
           case(LABEL):
                DestroyBRTL(wid);
                break; 
           case(SCROLLBAR):
                DestroySB(&(wid->scroll));
                break;
           case(TEXT):
                                XFreeGC(wid->text.dpy,wid->text.gc);
                XDestroyWindow(wid->text.dpy,wid->text.Win);
                free(wid->text.String);
                free(wid); 
                break;
           case(MENU):
                for(i=0 ; i< wid->menu.NextFree ; i++)
                    DestroyBRTL((XWidget *)wid->menu.List[i]);
      
                                XFreeGC(wid->menu.dpy,wid->menu.gc);
                XDestroyWindow(wid->menu.dpy,wid->menu.MenuWin);
                XDestroyWindow(wid->menu.dpy,wid->menu.Win);
                free(wid->menu.String);
                free(wid);
                break;
           case(LIST):
                for(i=0 ; i< wid->list.NextFree ; i++)
                    DestroyBRTL((XWidget *)wid->list.List[i]);

                DestroySB(wid->list.scroll);

                                XFreeGC(wid->list.dpy,wid->list.gc);
                XDestroyWindow(wid->list.dpy,wid->list.Win);
                free(wid);
                break;
          }
   }    
}
 
void DestroyShell(XGUI *main,XShell *sh)
{
 int i;

 if(sh != NULL && main != NULL && sh->destroyed != 1)
   {
     sh->destroyed = 1;

     for(i=0; i<sh->NextFree && i<MAXWIDGET ; i++)
         DestroyXWidget(sh->List[i]);

     /* Free back buffer if allocated */
     if(sh->BackBuffer != None)
        XFreePixmap(sh->dpy, sh->BackBuffer);

     XFreeGC(sh->dpy,sh->gc);
     XDestroyWindow(sh->dpy,sh->Win);

     /* The structure itself is freed by LoopShellEvent once the event loop
        stops dereferencing it (callbacks may still be on the stack). */
   }
}

void MapShell(XShell *sh)
{
 XEvent ev;

 if(sh != NULL)
   {
    XMapWindow(sh->dpy,sh->Win);
    XFlush(sh->dpy);
   }
}   

void ChangeShellName(XShell *sh,const char *name)
{
 if( sh != NULL)
    XStoreName(sh->dpy,sh->Win,name);
}


XShell *CreateWindow(XShell *sh,int x,int y,int w,int h)
{
 XShell *ret;
 XSetWindowAttributes attrs;

 if( sh==NULL)
    return(NULL);

 ret = (XShell *)calloc(1,sizeof(XShell));
 if( ret == NULL )
    return(NULL);
 
 ret->dpy = sh->dpy;

 ret->Win = XCreateSimpleWindow(sh->dpy,
                                sh->Win,
                                x, y,
                                w,h,
                                2,GetWhite(sh),GetBlack(sh));

 ret->BOCOLOR = sh->BOCOLOR;
 ret->BGCOLOR = sh->BGCOLOR;
 ret->FGCOLOR = sh->FGCOLOR;
 ret->SHCOLOR = sh->SHCOLOR;
 ret->gc = sh->gc;
 ret->BackBuffer = None;
 ret->Width = w;
 ret->Height = h;

 attrs.backing_store = Always;
 XChangeWindowAttributes(ret->dpy,ret->Win,CWBackingStore,&attrs);
 XSelectInput(ret->dpy,ret->Win,ButtonPressMask|ButtonReleaseMask|KeyReleaseMask|
                              KeyPressMask|PointerMotionMask|ExposureMask);
 XMapWindow(sh->dpy,ret->Win);
 return(ret);
}
