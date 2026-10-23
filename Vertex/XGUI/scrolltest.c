#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <signal.h>
#include "Connect.h"

XGUI gui;
XShell *sh;
XWidget *sc,*btn,*li;
int fail=0, clicked=0, cbcount=0;
int ox=0, oy=0;
int swx=0, swy=0, sww=0, swh=0;
int bwx=0, bwy=0;

extern void InternLoopEvent(XShell *,int,XEvent *);

void Scrolled(void){ cbcount++; }
void Clicked(void){ clicked++; }
void watchdog(int s){ (void)s; printf("WATCHDOG-HANG\n"); fflush(stdout); _exit(3); }

void Pump(void)
{
 XEvent ev;
 while(XEventsQueued(gui.dpy,QueuedAlready)>0)
   {
    XNextEvent(gui.dpy,&ev);
    InternLoopEvent(sh,1,&ev);
   }
}

void pumpwait(int us)
{
 int i;
 for(i=0;i<us/50000+1;i++){ usleep(50000); Pump(); }
}

int Pending(void)
{
 int n=0; XEvent ev;
 while(XEventsQueued(gui.dpy,QueuedAlready)>0){ XNextEvent(gui.dpy,&ev); n++; }
 return n;
}

void sendev(Window w,int type,unsigned int mask,int button,int x,int y,int state,int rx,int ry)
{
 XEvent e;
 memset(&e,0,sizeof(e));
 e.xany.type=type;
 e.xany.display=gui.dpy;
 e.xany.window=w;
 e.xbutton.time=CurrentTime;
 e.xbutton.button=button;
 e.xbutton.x=x; e.xbutton.y=y;
 e.xbutton.state=state;
 e.xbutton.x_root=rx; e.xbutton.y_root=ry;
 XSendEvent(gui.dpy,w,False,mask,&e);
 XFlush(gui.dpy);
 usleep(60000);
}

void press(Window w,int button,int x,int y){ sendev(w,ButtonPress,ButtonPressMask,button,x,y,256,x,y); }
void release(Window w,int button,int x,int y){ sendev(w,ButtonRelease,ButtonReleaseMask,button,x,y,0,x,y); }
void motion(Window w,int x,int y,int rx,int ry){ sendev(w,MotionNotify,PointerMotionMask,0,x,y,256,rx,ry); }

int nearv(float a,float b){ return fabs(a-b)<2.0; }

void GetWin(Window w,int *x,int *y,int *ww,int *hh)
{
 XWindowAttributes at; Window child;
 if(!XGetWindowAttributes(gui.dpy,w,&at)){ printf("ATTRFAIL\n"); fail=1; return; }
 XTranslateCoordinates(gui.dpy,w,DefaultRootWindow(gui.dpy),0,0,x,y,&child);
 if(ww) *ww=at.width; if(hh) *hh=at.height;
}

void Check(const char *name,int ok)
{
 printf("%s %s\n",ok?"PASS":"FAIL",name);
 if(!ok) fail=1;
 fflush(stdout);
}

void RoundTrip(void)
{
 float p; int v[]={0,100,37,63},i; int ok=1;
 for(i=0;i<4;i++)
   {
    SetScrollPos(sc,(float)v[i]);
    GetScrollPos(sc,&p);
    if(!nearv(p,(float)v[i])){ printf("  round %d -> %.2f\n",v[i],p); ok=0; }
   }
 SetScrollPos(sc,0.0);
 Check("Pos/Act round-trip (0,100,37,63)",ok);
}

/* External client: verifies pointer grab is held during the drag, then
   injects the ButtonRelease over to us from ITS OWN connection. */
void ExternalClient(Window trough)
{
 Display *d2; FILE *f; int i,st; Time t;
 if(!(d2=XOpenDisplay(NULL))) _exit(1);
 usleep(700000);
 st=XGrabPointer(d2,DefaultRootWindow(d2),False,0,
                 GrabModeAsync,GrabModeAsync,None,None,CurrentTime);
 f=fopen("/tmp/xgui_grabchk","w");
 if(f){ fprintf(f,"%d\n",st); fclose(f); }
 usleep(400000);
 {
  XEvent e;
  memset(&e,0,sizeof e);
  e.xany.type=ButtonRelease;
  e.xany.display=d2;
  e.xany.window=trough;
  e.xbutton.time=CurrentTime;
  e.xbutton.button=1;
  e.xbutton.y=100;
  XSendEvent(d2,trough,False,ButtonReleaseMask,&e);
  XFlush(d2);
 }
 usleep(500000);
 XCloseDisplay(d2);
 _exit(0);
}

int GrabStatus(void)
{
 FILE *f; int st=-1;
 f=fopen("/tmp/xgui_grabchk","r");
 if(f){ if(fscanf(f,"%d",&st)!=1) st=-1; fclose(f); }
 return st;
}

void ItemY(XWidget *lw,int idx,int *y)
{
 XWindowAttributes at; Window child; int xx;
 XTranslateCoordinates(gui.dpy,lw->list.List[idx]->Win,DefaultRootWindow(gui.dpy),0,0,&xx,y,&child);
}

/* List scroll: wheeling the list scrollbar must shift viewport items (no callback hijack) */
void ListScrollTest(void)
{
 int y0,y1,ok; Window troughWin;

 li=CreateList(sh,150,20,140,100);
 { int i; char nm[20]; for(i=0;i<20;i++){ sprintf(nm,"item%d",i); AddListItem(li,nm);} }
 MapWidget(li);
 pumpwait(600000);

 troughWin = li->list.scroll->ScrollWin;
 ItemY(li,5,&y0);

 press(troughWin,5,7,50); release(troughWin,5,7,50); pumpwait(200000);
 press(troughWin,5,7,50); release(troughWin,5,7,50); pumpwait(200000);
 ItemY(li,5,&y1);

 ok = (y1 < y0);
 if(!ok) printf("  item5 y %d -> %d\n",y0,y1);
 Check("list viewport scrolls via its scrollbar (Owner-bound callback)",ok);

 DestroyShell(&gui,sh);
 sh=CreateShell(&gui,"ScrollTest2",60,100,320,240);
 sc=CreateScroll(sh,10,10,15,150,VSCROLL);
 AddCallBack(sc,Scrolled);
 btn=CreateButton(sh,"Btn",40,10,60,20);
 AddCallBack(btn,Clicked);
 MapShell(sh); MapAllWidget(sh);
 pumpwait(400000);
 GetWin(sc->scroll.ScrollWin,&swx,&swy,&sww,&swh);
 GetWin(btn->button.Win,&bwx,&bwy,0,0);
}

/* Drag scenario: ButtonPress enters PressScroll's modal loop (grab held);
   motion arrives; the ButtonRelease is delivered by the external client
   (standing in for a release occurring while the pointer is elsewhere);
   loop must terminate and release the grab. */
void GrabTerminationTest(void)
{
 float p0,p1; int st,ok;

 unlink("/tmp/xgui_grabchk");
 SetScrollPos(sc,30.0);
 GetScrollPos(sc,&p0);
 cbcount=0;

 { pid_t pid=fork();
   if(pid==0){ ExternalClient(sc->scroll.ScrollWin); _exit(0); } }

 press(sc->scroll.ScrollWin,1,7,35);        /* on the thumb (Act~39, 39..57) */
 motion(sc->scroll.ScrollWin,7,60,swx+7,swy+60);
 pumpwait(2500000);

 GetScrollPos(sc,&p1);
 st=GrabStatus();

 ok = (st==1) && Pending()==0 && (p1!=p0) && (cbcount>=1);
 if(!ok) printf("  grab_status=%d (1=AlreadyGrabbed) pend=%d p0=%.2f p1=%.2f cbs=%d\n",st,Pending(),p0,p1,cbcount);
 Check("drag: XGrabPointer held; release from other client terminates loop",ok);

 {
  Display *d3=XOpenDisplay(NULL);
  if(d3){
   int g=XGrabPointer(d3,DefaultRootWindow(d3),False,0,GrabModeAsync,GrabModeAsync,None,None,CurrentTime);
   printf("  grab after drag-end status=%d (0=Success: grab released)\n",g);
   if(g==0) XUngrabPointer(d3,CurrentTime);
   XCloseDisplay(d3);
   Check("pointer grab released after drag",g==0);
  }
 }

 clicked=0;
 press(btn->button.Win,1,10,10);
 release(btn->button.Win,1,10,10);
 pumpwait(200000);
 Check("app responsive after drag",clicked>=1 && Pending()==0);
}

int Viewable(Window w)
{
 XWindowAttributes at;
 if(!XGetWindowAttributes(gui.dpy,w,&at)) return 0;
 return (at.map_state == IsViewable);
}

void FileSelectorScrollTest(void)
{
 XWidget *li2;
 int i,y,lx,ly,lw,lh,lasty,ok;
 char nm[20];
 Window child; Window troughWin;

 li2=CreateList(sh,10,120,140,100);
 MapWidget(li2);
 pumpwait(400000);
 troughWin = li2->list.scroll->ScrollWin;

 Check("empty/short list: scrollbar hidden",Viewable(troughWin)==0);

 for(i=0;i<4;i++){ sprintf(nm,"f%d",i); AddListItem(li2,nm); }
 pumpwait(300000);
 Check("short list (fits): still no scrollbar",Viewable(troughWin)==0);

 for(i=4;i<12;i++){ sprintf(nm,"f%d",i); AddListItem(li2,nm); }
 pumpwait(300000);
 Check("overflowing list: scrollbar becomes visible",Viewable(troughWin)==1);

 for(i=0;i<15;i++)
   { press(troughWin,5,7,90); release(troughWin,5,7,90); pumpwait(80000); }
 pumpwait(300000);

 XTranslateCoordinates(gui.dpy,li2->list.Win,DefaultRootWindow(gui.dpy),0,0,&lx,&ly,&child);
 lw = li2->list.Width-15; lh = li2->list.Height;
 ItemY(li2,11,&lasty);
 ok = (lasty >= ly) && (lasty <= ly+lh-4);
 if(!ok) printf("  list at %d size %d, last item y=%d\n",ly,lh,lasty);
 Check("last entry reachable via wheel without resizing",ok);

 press(li2->list.List[11]->Win,1,10,5);
 release(li2->list.List[11]->Win,1,10,5);
 pumpwait(300000);
 { char *sel=NULL; int idx=GetActiveListItem(li2,&sel);
   ok = (idx==11 && sel && strcmp(sel,"f11")==0);
   if(!ok) printf("  active idx=%d sel=%s\n",idx,sel?sel:"(null)");
   Check("selection works after scrolling",ok); }

 DestroyAllListItem(li2);
 pumpwait(300000);
 Check("after DestroyAllItems: scrollbar hidden again",Viewable(troughWin)==0);

 for(i=0;i<3;i++){ sprintf(nm,"g%d",i); AddListItem(li2,nm); }
 pumpwait(300000);
 ItemY(li2,0,&lasty);
 ok = (lasty >= ly-2) && Viewable(troughWin)==0;
 if(!ok) printf("  item0 y=%d listy=%d view=%d\n",lasty,ly,Viewable(troughWin));
 Check("viewport reset to top after DestroyAllItems",ok);
}

int main(void)
{
 float p; int i;

 signal(SIGALRM,watchdog);
 alarm(90);

 if(XConnect(&gui,"")==0){ printf("CONNFAIL\n"); return 1; }
 sh=CreateShell(&gui,"ScrollTest",60,100,320,240);
 sc=CreateScroll(sh,10,10,15,150,VSCROLL);
 AddCallBack(sc,Scrolled);
 btn=CreateButton(sh,"Btn",40,10,60,20);
 AddCallBack(btn,Clicked);
 MapShell(sh); MapAllWidget(sh);
 pumpwait(600000);
 GetWin(sh->Win,&ox,&oy,0,0);
 GetWin(sc->scroll.ScrollWin,&swx,&swy,&sww,&swh);
 GetWin(btn->button.Win,&bwx,&bwy,0,0);
 printf("WIN shell=%d,%d trough=%d,%d,%dx%d btn=%d,%d\n",ox,oy,swx,swy,sww,swh,bwx,bwy);
 fflush(stdout);

 RoundTrip();

 clicked=0;
 press(btn->button.Win,1,10,10);
 release(btn->button.Win,1,10,10);
 pumpwait(200000);
 Check("button callback via ButtonPress",clicked>=1);

 SetScrollPos(sc,0.0);
 press(sh->Win,1,250,210); release(sh->Win,1,250,210); pumpwait(100000);
 press(sh->Win,1,250,210); release(sh->Win,1,250,210); pumpwait(100000);
 GetScrollPos(sc,&p);
 Check("clicks elsewhere: no stray position change",p==0.0);

 cbcount=0;
 press(sc->scroll.ScrollWin,5,7,140);
 release(sc->scroll.ScrollWin,5,7,140);
 pumpwait(200000);
 GetScrollPos(sc,&p);
 Check("wheel down: one step, one callback, no modal loop",p>0.0 && p<30.0 && cbcount==1);

 cbcount=0;
 press(sc->scroll.ScrollWin,4,7,140);
 release(sc->scroll.ScrollWin,4,7,140);
 pumpwait(200000);
 GetScrollPos(sc,&p);
 Check("wheel up returns to 0",p==0.0 && cbcount==1);

 SetScrollPos(sc,0.0);
 cbcount=0;
 press(sc->scroll.ScrollWin,1,7,140);
 release(sc->scroll.ScrollWin,1,7,140);
 pumpwait(200000);
 GetScrollPos(sc,&p);
 Check("trough press below thumb: page-jump + release exits loop",p>0.0 && p<30.0 && cbcount==1 && Pending()==0);

 for(i=0;i<4;i++)
   { press(sc->scroll.ScrollWin,1,7,140); release(sc->scroll.ScrollWin,1,7,140); pumpwait(100000); }
 GetScrollPos(sc,&p);
 Check("5 cumulative page-jumps pass 50%",p>50.0 && p<=100.0);

 for(i=0;i<6;i++)
   { press(sc->scroll.ScrollWin,1,7,148); release(sc->scroll.ScrollWin,1,7,148); pumpwait(100000); }
 GetScrollPos(sc,&p);
 Check("clamped at 100 after over-jumping",p>=95.0 && p<=100.0);

 SetScrollPos(sc,0.0);
 cbcount=0;
 press(sc->scroll.ScrollWin,1,7,1);
 release(sc->scroll.ScrollWin,1,7,1);
 pumpwait(150000);
 GetScrollPos(sc,&p);
 Check("press on thumb does not jump, no callback",p==0.0 && cbcount==0);

 cbcount=0;
 press(sc->scroll.ScrollWin,1,7,1);
 motion(sc->scroll.ScrollWin,7,31,1000,731);
 motion(sc->scroll.ScrollWin,7,61,1000,761);
 release(sc->scroll.ScrollWin,1,7,61);
 pumpwait(200000);
 GetScrollPos(sc,&p);
 Check("motion drag moves thumb by root delta",p>20.0 && p<60.0);

 SetScrollPos(sc,25.0);
 cbcount=0;
 press(sc->scroll.ScrollWin,1,7,40);
 release(sc->scroll.ScrollWin,1,7,40);
 pumpwait(150000);
 Check("release without movement: no callback",cbcount==0);

 SetScrollPos(sc,0.0);
 cbcount=0;
 press(sc->scroll.ScrollWin,2,7,140);
 release(sc->scroll.ScrollWin,2,7,140);
 pumpwait(150000);
 GetScrollPos(sc,&p);
 Check("middle button ignored",p==0.0 && cbcount==0);

 ListScrollTest();

 GrabTerminationTest();

 FileSelectorScrollTest();

 DestroyShell(&gui,sh);
 XDeconnect(&gui);
 printf(fail?"TESTFAIL\n":"TESTPASS\n");
 fflush(stdout);
 return fail;
}
