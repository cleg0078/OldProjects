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

void RedrawText(XTextWidget *wid)
{
 int posx,posy;

 if(wid == 0)
    return;

 posx = (wid->Width/2) - XTextWidth(wid->Font,wid->String,strlen(wid->String))/2;
 if( posx < 0 )
    posx = 0;

 posy = (wid->Height/2) + (wid->Font->ascent+wid->Font->descent-3)/2;
 XClearWindow(wid->dpy,wid->Win);
 XSetForeground(wid->dpy,wid->gc,wid->BOCOLOR);
 XFillRectangle(wid->dpy,wid->Win,wid->gc,0,0,wid->Width,wid->Height);
 XSetForeground(wid->dpy,wid->gc,wid->SHCOLOR);
 XFillRectangle(wid->dpy,wid->Win,wid->gc,2,2,wid->Width-4,wid->Height-4);
 XSetForeground(wid->dpy,wid->gc,wid->FGCOLOR);
 XFillRectangle(wid->dpy,wid->Win,wid->gc,3,3,wid->Width-5,wid->Height-5);
 XSetForeground(wid->dpy,wid->gc,wid->BGCOLOR);
 XDrawString(wid->dpy,wid->Win,wid->gc,posx,posy,wid->String,strlen(wid->String));
} 

void PressText(XTextWidget *wid)
{
 char test[2];
 KeySym key = 0;
 test[0] = 0;
 XEvent ev;

 if(wid == NULL)
    return;
 
 XFlush(wid->dpy);

 wid->String[wid->Actual] ='|';
 RedrawText(wid);
 
 do
  {
   XNextEvent (wid->dpy, &ev);

   if(ev.type == KeyPress )
     {
      XLookupString ((XKeyEvent *)&ev, test, 1, &key, NULL);

      if(key != XK_Return && key != XK_Delete && key != XK_Shift_Lock)
        {
         if(wid->Actual<MAXTEXT)
           {
            wid->String[wid->Actual]= 0;
            wid->String[wid->Actual++] = test[0];
            wid->String[wid->Actual] = '|';
                        
            RedrawText(wid);
            usleep(150);
            XFlush(wid->dpy);
           }
        }
              
      if( key == XK_Delete )
        {
         wid->String[wid->Actual] = 0;

         wid->Actual--;
         if(wid->Actual < 0 )
            wid->Actual = 0;
      
         wid->String[wid->Actual] = '|';
 
         RedrawText(wid);
         usleep(120);
         XFlush(wid->dpy);
        }
     }    
 }while( key != XK_Return && key != XK_KP_Enter && wid->Actual<MAXTEXT  );


 wid->String[wid->Actual] =0; 
 RedrawText(wid);
 if( wid->CallBack )
    wid->CallBack();
}

void ConstPress(XTextWidget *w)
{
 /* Constant Text */
}

XWidget *CreateCMText(int type,XShell *sh,const char *txt,int x,int y,int tx,int ty)
{
 XWidget *wid;  
 Display *dpy;
 XGCValues gcv;
 XSetWindowAttributes attrs;

 wid = (XWidget *)calloc(sizeof(XTextWidget),1);
 if(wid == NULL) 
    return(NULL);

 if( sh == NULL )
   {
    free(wid);
    return(NULL);
   }

 if( (wid->text.String = (char *)calloc(MAXTEXT+2,1)) == NULL)
    {
      free(wid);
      return(NULL);
    }

 dpy = wid->text.dpy = sh->dpy;
 wid->text.Redraw = &RedrawText;
 wid->text.Press = (type==1)?&ConstPress:&PressText;
 wid->text.Type = TEXT; 
 wid->text.Width = tx;
 wid->text.Height = ty;
 wid->text.BGCOLOR = sh->BGCOLOR;
 wid->text.FGCOLOR = sh->FGCOLOR;
 wid->text.SHCOLOR = sh->SHCOLOR;
 wid->text.BOCOLOR = sh->BOCOLOR;

 memset(wid->text.String,0,MAXTEXT);
 strncpy(wid->text.String,txt,MAXTEXT);
 wid->text.String[MAXTEXT] = 0;
 wid->text.Actual = strlen(wid->text.String);

 wid->text.Win = XCreateSimpleWindow(dpy,
                                     sh->Win,
                                     x, y,
                                     tx,ty,
                                     1,wid->text.BGCOLOR,wid->text.FGCOLOR);

 attrs.backing_store = Always;
 XChangeWindowAttributes(wid->text.dpy,wid->text.Win,CWBackingStore,&attrs);

 gcv.function = GXcopy;
 wid->text.Font = XGUIGetFont(dpy);
 if(wid->text.Font == NULL)
   {
    XDestroyWindow(dpy,wid->text.Win);
    free(wid->text.String);
    free(wid);
    XFlush(dpy);
    return(NULL);
   }
 gcv.font = wid->text.Font->fid;

 wid->text.gc = XCreateGC(dpy,wid->text.Win,GCFunction|GCFont,&gcv);
 XSelectInput(dpy,wid->text.Win,KeyPress|KeyRelease|ButtonPress|ButtonReleaseMask);
 XSetForeground(dpy,wid->text.gc,wid->text.BOCOLOR);
 XFlush(dpy);
 return(wid);
}



XWidget *CreateText(XShell *sh,const char *txt,int x,int y,int tx,int ty)
{
 XWidget *wid;

 if( sh == NULL || sh->NextFree >= MAXWIDGET )
    return(NULL);

 wid = (XWidget *)CreateCMText(0,sh,txt,x,y,tx,ty);

 if( wid )
   sh->List[sh->NextFree++] = wid;

 return(wid);
}

XWidget *CreateConstText(XShell *sh,const char *txt,int x,int y,int tx,int ty)
{
 XWidget *wid;

 if( sh == NULL || sh->NextFree >= MAXWIDGET )
    return(NULL);

 wid = (XWidget *)CreateCMText(1,sh,txt,x,y,tx,ty);

 if( wid )
   sh->List[sh->NextFree++] = wid;

 return(wid);
}

void SetTextString(XWidget *wid,char *txt)
{
 if(wid != NULL)
   if( wid->any.Type == TEXT)
     {
      memset(wid->text.String,0,MAXTEXT);
      strncpy(wid->text.String,txt,MAXTEXT);
      wid->text.String[MAXTEXT] = 0;
      wid->text.Actual = strlen(wid->text.String);
      wid->text.Redraw(&(wid->text));
     }
}

char *GetTextString(XWidget *wid)
{
 if(wid != NULL)
    if(wid->any.Type == TEXT)
       return(wid->text.String);

 return(NULL);
}
