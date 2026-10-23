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

extern void ScrollStep(XScrollWidget *,int);
extern void ListScrollVis(XWidget *);
extern int ListOwnsWindow(XListWidget *,Window);

void AddCallBack(XWidget *wid,void fct())
{
 if( wid)
    switch( wid->any.Type)
          {
           case(COMMAND):      wid->button.CallBack   = fct; break;
           case(REPEATER):     wid->button.CallBack   = fct; break; 
           case(LIST):         wid->list.CallBack     = fct; break;
           case(SCROLLBAR):    wid->scroll.CallBack   = fct; break;
           case(TOGGLE):       wid->toggle.CallBack   = fct; break;
           case(TEXT):         wid->text.CallBack     = fct;
          }
}

void MapWidget(XWidget *wid)
{
 if( wid )
   {
    switch(wid->any.Type)
          {
           case(SCROLLBAR):
                XMapWindow(wid->scroll.dpy,wid->scroll.ScrollWin);
                XMapSubwindows(wid->scroll.dpy,wid->scroll.ScrollWin);
                break;
           case(COMMAND):
           case(REPEATER):
           case(MENU):
           case(TOGGLE):
           case(LABEL):
           case(CHECKBOX):
                XMapWindow(wid->any.dpy,wid->any.Win);
                break;
           case(LIST):
                XMapWindow(wid->list.dpy,wid->list.Win);
                XMapSubwindows(wid->list.dpy,wid->list.Win);
                ListScrollVis(wid);
                break;
            case(TEXT):
                XMapWindow(wid->any.dpy,wid->any.Win);
                wid->text.Redraw(&(wid->text));
		break;
          }
 
    XFlush(wid->any.dpy);
   }
}


void MapAllWidget(XShell *sh)
{
 int i;
 
 if( sh ) {
	 for(i=0;i<sh->NextFree ; i++)
	     MapWidget(sh->List[i]);
 }
}

void InternLoopEvent(XShell *sh,int Check,XEvent *ev)
{
 int i,bpress;

 if( sh == NULL || sh->destroyed == 1 )
    return;

 switch(ev->type)
       {
        case(Expose):
        case(CreateNotify): 
        case(DestroyNotify):
             for(i=0; i<sh->NextFree; i++)
                if( sh->List[i])
                  {
                    switch( sh->List[i]->any.Type)
                      {
                       case(TEXT):
                            sh->List[i]->text.Redraw(&(sh->List[i]->text));
                            break;
                      }
                 }
            break;
         case(ButtonPress):
            bpress = 1;
            if( Check )
              {
               for(i=0; i<sh->NextFree; i++)
                  {
                   if( sh->List[i] == NULL )
                      continue;
                   switch(sh->List[i]->any.Type) 
                         {
                          case(COMMAND):
                               if( ev->xbutton.window == sh->List[i]->any.Win) 
                                  sh->List[i]->button.Press(&(sh->List[i]->button));
                               break;
                          case(REPEATER):
                               if( ev->xbutton.window == sh->List[i]->any.Win)
                                  sh->List[i]->repeater.Press((XButtonWidget *)&(sh->List[i]->repeater));  
                               break;
                         case(MENU):
                              if( ev->xbutton.window == sh->List[i]->any.Win)
                                 sh->List[i]->menu.Press(&(sh->List[i]->menu));
                              break; 
                         case(SCROLLBAR):
                              if( ev->xbutton.button == 1 &&
                                  (ev->xbutton.window == sh->List[i]->scroll.Win ||
                                   ev->xbutton.window == sh->List[i]->scroll.ScrollWin) )
                               {
                                XPutBackEvent(sh->dpy,ev);
                                sh->List[i]->scroll.Press(&(sh->List[i]->scroll));
                               }
                              else
                               if( (ev->xbutton.button == 4 || ev->xbutton.button == 5) &&
                                   (ev->xbutton.window == sh->List[i]->scroll.Win ||
                                    ev->xbutton.window == sh->List[i]->scroll.ScrollWin) )
                                  ScrollStep(&(sh->List[i]->scroll),(ev->xbutton.button==4)?-1:1);
                              break;   
                         case(TEXT):
                              if( ev->xbutton.window == sh->List[i]->any.Win) 
                                 sh->List[i]->text.Press(&(sh->List[i]->text));
                              break; 
                         case(TOGGLE):
                              if( ev->xbutton.window == sh->List[i]->any.Win)
                                 sh->List[i]->toggle.Press(&(sh->List[i]->toggle)); 
                              break;
                         case(LIST):
                              if( (ev->xbutton.button == 4 || ev->xbutton.button == 5) &&
                                  sh->List[i]->list.scroll != NULL &&
                                  (ev->xany.window == sh->List[i]->list.scroll->Win ||
                                   ev->xany.window == sh->List[i]->list.scroll->ScrollWin ||
                                   ListOwnsWindow(&(sh->List[i]->list),ev->xany.window)) )
                               {
                                ScrollStep(sh->List[i]->list.scroll,(ev->xbutton.button==4)?-1:1);
                                break;   /* wheel never toggles an item */
                               }
                              else
                               {
                                if( ev->xbutton.button == 1 &&
                                    sh->List[i]->list.scroll != NULL &&
                                    (ev->xany.window == sh->List[i]->list.scroll->Win ||
                                     ev->xany.window == sh->List[i]->list.scroll->ScrollWin) )
                                   XPutBackEvent(sh->dpy,ev);
                                sh->List[i]->list.Press(&(sh->List[i]->list),ev->xany.window);  
                               }
                              break;
                         case(CHECKBOX):
                              if( ev->xbutton.window == sh->List[i]->any.Win)  
                                 sh->List[i]->check.Press(&(sh->List[i]->check));

                        } 
                   if( sh->destroyed == 1 )
                      break;   /* callback destroyed the shell: widgets gone */
                  }
             }
       }
}
  

/* Legacy single-shell loop: it ignores the deferred-destroy protocol and
   the gui-level dispatch that LoopShellEvent() provides. No caller in the
   current tree; new code must use LoopShellEvent (audited 2026). */
void LoopEvent(XShell *sh)
{
 XEvent ev;

 while(1)
      {
        if( sh)
          {
           XNextEvent(sh->dpy,&ev);
           InternLoopEvent(sh,1,&ev);
          }
      }
}

void LoopShellEvent(XGUI *gui,XShell *sh)
{
 XEvent ev;
 int i,cont=1;

 if(sh != NULL)
    sh->activeLoops++;

 while(cont)
    if(sh && !sh->destroyed)
      {
       XNextEvent(sh->dpy,&ev);

       for(i=0;i<MAXSHELL ;i++)
          if(gui->List[i] != NULL)
             InternLoopEvent(gui->List[i],(gui->List[i] == sh)?1:0,&ev);

       /* Reap shells whose callbacks called DestroyShell: a shell is freed
          only when no event loop still references it (activeLoops == 0), so
          destroying ANOTHER shell from a callback cannot pull the rug from
          under that shell's own (nested) loop. */
       for(i=0;i<MAXSHELL ;i++)
          if( gui->List[i] != NULL && gui->List[i]->destroyed == 1 &&
              gui->List[i]->activeLoops <= 0 )
            {
             free(gui->List[i]);
             gui->List[i] = NULL;
            }

       XFlush(sh->dpy);
      }
    else
       cont = 0;

 if(sh != NULL)
   {
    sh->activeLoops--;
    if( sh->destroyed == 1 && sh->activeLoops <= 0 )
      {
       for(i=0;i<MAXSHELL ;i++)
          if( gui->List[i] == sh )
             gui->List[i] = NULL;
       free(sh);
      }
   }
}  
