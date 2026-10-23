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


/* 1 if win is the list container or one of the item windows */
int ListOwnsWindow(XListWidget *wid,Window win)
{
 int i;

 if( wid->Win == win )
    return(1);

 for(i=0 ; i<wid->NextFree ; i++)
     if(wid->List[i]->Win == win )
        return(1);

 return(0);
}


int CheckList(XListWidget *wid,Window win){
 int i,find=0;

 /* The pressed Window Is include in The List Window */
 for(i=0 ; i<wid->NextFree ; i++)
     if(wid->List[i]->Win == win )
        find = 1;

 if( find == 1)
 for(i=0 ; i<wid->NextFree ; i++)
    {
     if(wid->List[i]->Win != win )
        wid->List[i]->State = ACTIVE;

     wid->List[i]->Press(wid->List[i]);
    }

 return(find);
}


extern XScrollWidget *XCurrentScroll;

static void ListScrollCB()
{
 XListWidget *wid1;
 int i,sy,dy;

 if( XCurrentScroll == NULL || XCurrentScroll->Owner == NULL )
    return;

 wid1 = (XListWidget *)XCurrentScroll->Owner;

 if( wid1->Maxy >= wid1->Height)
    {
      sy = (int)((wid1->Maxy-wid1->Height) * XCurrentScroll->Pos / 100.0);
      for(dy=-sy,i=0; i<wid1->NextFree ; dy+=wid1->List[i]->Height,i++)
          XMoveWindow(wid1->dpy,wid1->List[i]->Win,0,dy);
     }
}
  
void PressList(XListWidget *wid,Window win)
{
 if( wid->scroll != NULL &&
     (win == wid->scroll->Win || win == wid->scroll->ScrollWin) )
   {
     wid->scroll->Press(wid->scroll);
   }
 else
   {
    if( CheckList(wid,win) && wid->CallBack )
       wid->CallBack();
   }
}

void ListScrollVis(XWidget *wid)
{
 XScrollWidget *sc;

 if( wid == NULL || wid->any.Type != LIST || wid->list.scroll == NULL )
    return;

 sc = wid->list.scroll;

 if( wid->list.Maxy > wid->list.Height )
   {
    XMapWindow(sc->dpy,sc->ScrollWin);
    XMapSubwindows(sc->dpy,sc->ScrollWin);
   }
 else
    XUnmapWindow(sc->dpy,sc->ScrollWin);

 XFlush(sc->dpy);
}

void ListScrollReset(XWidget *wid)
{
 if( wid == NULL || wid->any.Type != LIST || wid->list.scroll == NULL )
    return;

 wid->list.scroll->Pos = wid->list.scroll->Min;
 wid->list.scroll->Act = 0;
 XMoveWindow(wid->list.dpy,wid->list.scroll->Win,0,0);
 XFlush(wid->list.dpy);
}

XWidget *CreateList(XShell *sh,int x,int y,int tx,int ty)
{
 XWidget *wid;  
 Display *dpy;
 XGCValues gcv;

 wid = (XWidget *)calloc(sizeof(XListWidget),1);
 if(wid == NULL) 
    return(NULL);

 if( sh == NULL || sh->NextFree >= MAXWIDGET )
   {
    free(wid);
    return(NULL);
   }

 dpy = wid->menu.dpy = sh->dpy;
 sh->List[sh->NextFree++] = wid;
 {
  /* scrollbar column scales with the screen like the rest of the UI */
  int sbw = (int)(15 * XGUIScreenScale(dpy));
  if( sbw < 15 )
     sbw = 15;
  wid->list.Maxx = tx-sbw;
  wid->list.Height = ty;
  wid->list.Width = tx;
  wid->list.Win = XCreateSimpleWindow(dpy,sh->Win,x,y,tx-sbw,ty,1,sh->BGCOLOR,sh->FGCOLOR);
  wid->list.scroll = (XScrollWidget *)CreateVHScroll(sh,sh->dpy,sh->Win,x+tx-sbw,y,sbw,ty,VSCROLL);
 }
 wid->list.Press = &PressList;
 wid->list.Type = LIST; 
 wid->list.NextFree = 0;
 wid->list.Maxy = 0;
 wid->list.CallBack = NULL;
 wid->list.BGCOLOR = sh->BGCOLOR;
 wid->list.FGCOLOR = sh->FGCOLOR;
 wid->list.SHCOLOR = sh->SHCOLOR;
 wid->list.BOCOLOR = sh->BOCOLOR;

 if( wid->list.scroll != NULL )
   {
    wid->list.scroll->Owner = (void *)wid;
    wid->list.scroll->CallBack = ListScrollCB;
   }

 gcv.function = GXcopy;
 wid->list.Font = XGUIGetFont(dpy);
 gcv.font = (wid->list.Font != NULL) ? wid->list.Font->fid : XLoadFont(dpy,DFONT);
 wid->list.gc = XCreateGC(dpy,wid->list.Win,GCFunction|GCFont,&gcv);
 XSelectInput(dpy,wid->list.Win,ButtonPress|ButtonReleaseMask);
 XSetForeground(dpy,wid->list.gc,sh->BOCOLOR);

 XFlush(dpy);
 return(wid);
}


XWidget *AddListItem(XWidget *wid, const char *txt)
{
 XWidget *act;
 int i,tx,posy,ty;
 if(wid == NULL || wid->any.Type != LIST || wid->list.NextFree >= 200 ||
    wid->list.Font == NULL)
    return(NULL);
    
 ty = wid->list.Font->ascent+wid->list.Font->descent+6;
 posy = (wid->list.NextFree)*ty;
 wid->list.Maxy +=ty;
 
 tx = XTextWidth(wid->list.Font,txt,strlen(txt))+10;

/*
 if( wid->list.Maxx < tx ) 
   {
     wid->list.Maxx = tx;
     for(i=0; i<wid->list.NextFree && i<200 ; i++)
        {
         XResizeWindow(wid->list.dpy,wid->list.List[i]->Win,wid->list.Maxx,ty);
         wid->list.List[i]->Width = wid->list.Maxx;
         wid->list.List[i]->Resize(wid->list.List[i]);
        }
   }
*/

 act = (XWidget *)CreateGToggle(wid->list.dpy,wid->list.Win,txt,0,posy,wid->list.Maxx,ty,0,LEFT,wid->list.BGCOLOR,wid->list.FGCOLOR,wid->list.BOCOLOR,wid->list.SHCOLOR);
 if(act != NULL)
   {
    wid->list.List[wid->list.NextFree++] = (XToggleWidget *)act;
    XMapWindow(act->any.dpy,act->any.Win);
    ListScrollVis(wid);
   }

 return(act);
}

void DestroyListItem(XWidget *wid,int nb)
{
 int i,py;
 XToggleWidget *tmp;

 if( wid == NULL || wid->any.Type != LIST)
    return;

 if(nb<0 || nb>=wid->list.NextFree || wid->list.NextFree==0)
    return;          /* List Item Not In Range */

 if( wid->list.List[nb] == NULL)
    return;

 tmp = wid->list.List[nb];
 wid->list.Maxy -= wid->list.List[nb]->Height;
 XFreePixmap(tmp->dpy,tmp->Fore);
/* Font is the shared XGUIGetFont() cache: not owned, not freed */
 XFreeGC(tmp->dpy,tmp->gc);
 XDestroyWindow(tmp->dpy,tmp->Win);
 free(tmp);

 for(py=i=0; i<nb ; i++)
     py += wid->list.List[i]->Height;

 for(i=nb+1; i<wid->list.NextFree ; py+=wid->list.List[i]->Height,i++)
     XMoveWindow(wid->list.dpy,wid->list.List[i]->Win,0,py);

 for(i=nb ; i<wid->list.NextFree ; i++ )
     wid->list.List[i] = wid->list.List[i+1];
 
 wid->list.List[--wid->list.NextFree] = NULL;
 ListScrollVis(wid);
}     


void DestroyAllListItem(XWidget *wid)
{
 int i,max;
 XToggleWidget *tmp;

 if(wid == NULL || wid->any.Type != LIST)
    return;
 
 
 for(i=0,max=wid->list.NextFree; i<max ; i++ )
    {
     tmp = wid->list.List[i];
     XFreePixmap(tmp->dpy,tmp->Fore);
     XFreeGC(tmp->dpy,tmp->gc);
     XDestroyWindow(tmp->dpy,tmp->Win);
     free(tmp);
     wid->list.List[i] = NULL;
    }
 
 wid->list.NextFree = 0;
 wid->list.Maxy = 0;
 /* same scaled scrollbar-column deduction as CreateList (was fixed -20) */
 {
  int sbw = (int)(15 * XGUIScreenScale(wid->list.dpy));
  if( sbw < 15 )
     sbw = 15;
  wid->list.Maxx = wid->list.Width-sbw;
 }
 ListScrollReset(wid);
 ListScrollVis(wid);
}

int GetActiveListItem(XWidget *wid,char **tmp)
{
 int i;

 if(wid==NULL || wid->any.Type !=LIST)
    return(-1);
 else
   {
    for(i=0 ; i< wid->list.NextFree ; i++)
        if(wid->list.List[i]->State == ACTIVE )
          {
           *tmp = wid->list.List[i]->String; 
           return(i);
          }
    return(-1);
   }    
}

int ActiveListItem(XWidget *wid,char *n)
{
 int find,i;

 if( wid->any.Type != LIST )
    return(-1);

 for(find=-1,i=0 ; i< wid->list.NextFree ; i++)
    if( strcmp(n,wid->list.List[i]->String) == 0 )
       find = i;
    else
       if( wid->list.List[i]->State == ACTIVE )
          wid->list.List[i]->Press(wid->list.List[i]);
 
 if( find >-1 && wid->list.List[find]->State == NOTACTIVE )
    wid->list.List[find]->Press(wid->list.List[find]);

 return(find);
}

int UnActiveListItem(XWidget *wid,char *n)
{
 int find,i;

 if( wid->any.Type != LIST )
    return(-1);

 for(find=-1,i=0 ; i< wid->list.NextFree ; i++)
    if( strcmp(n,wid->list.List[i]->String) == 0 )
       find = i;

 if( find >-1 && wid->list.List[find]->State == ACTIVE )
    wid->list.List[find]->Press(wid->list.List[find]);

 return(find);
}

int ActivePosListItem(XWidget *wid,int pos)
{
 int i;

 if( wid->any.Type != LIST || pos >= wid->list.NextFree || pos<0 )
    return(-1);

 wid->list.List[pos]->State = NOTACTIVE;
 wid->list.List[pos]->Press(wid->list.List[pos]); 
 return(1);
}

