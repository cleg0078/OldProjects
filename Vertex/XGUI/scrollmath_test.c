#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

/* Scroll math functions under test never touch the server; link real Xlib for
   symbols referenced by PressScroll (never invoked here) */
#include "Scroll.c"

static int fail=0;

void Check(const char *name,int ok)
{
 printf("%s %s\n",ok?"PASS":"FAIL",name);
 if(!ok) fail=1;
}

int nearv(float a,float b){ return fabsf(a-b)<0.75f; }

int main(void)
{
 XScrollWidget v,h,tiny;
 int i, ok;
 float p;

 memset(&v,0,sizeof v);
 v.Type=SCROLLBAR; v.Orientation=VSCROLL; v.Width=15; v.Height=150;
 v.Min=0.0; v.Max=100.0;

 memset(&h,0,sizeof h);
 h.Type=SCROLLBAR; h.Orientation=HSCROLL; h.Width=50; h.Height=50;
 h.Min=0.0; h.Max=100.0;

 memset(&tiny,0,sizeof tiny);
 tiny.Type=SCROLLBAR; tiny.Orientation=VSCROLL; tiny.Width=4; tiny.Height=4;
 tiny.Min=0.0; tiny.Max=100.0;

 Check("range math (150px -> 130)", ScrollRange(&v)==130);
 Check("range math (50px -> 42)", ScrollRange(&h)==42);
 Check("tiny bar range guard (4px -> >=1)", ScrollRange(&tiny)>=1);

 /* No division by zero for degenerate sizes */
 v.Height=2; ScrollRange(&v); v.Height=150;
 v.Act=5; ClampAct(&v); UpdatePos(&v);
 Check("degenerate dpl does not crash",1);

 /* Round-trip Act -> Pos -> Act for default Min/Max */
 ok=1;
 for(i=0;i<=130;i+=7)
   {
    v.Act=i; UpdatePos(&v);
    if(abs(ActFromPos(&v,v.Pos)-i)>1) ok=0;
   }
 Check("Act->Pos->Act round-trip (0..100%)",ok);

 /* Endpoints exact */
 v.Act=0; UpdatePos(&v);
 Check("Pos(Act=0)==Min",v.Pos==0.0f);
 v.Act=130; UpdatePos(&v);
 Check("Pos(Act=range)==Max",nearv(v.Pos,100.0f));

 /* Clamp */
 v.Act=-50; ClampAct(&v);
 Check("ClampAct floor 0",v.Act==0);
 v.Act=9999; ClampAct(&v);
 Check("ClampAct ceiling range",v.Act==130);

 /* Non-default Min/Max honoured */
 v.Min=10.0; v.Max=20.0; v.Act=130; UpdatePos(&v);
 Check("Pos maps to Max when Min=10 Max=20",nearv(v.Pos,20.0f));
 v.Act=ActFromPos(&v,15.0f);
 Check("ActFromPos midpoint",v.Act>=64 && v.Act<=66);

 /* Pos never escapes [Min,Max] even with stale Act */
 v.Act=200; UpdatePos(&v);
 Check("Pos clamped to Max on stale overflow",v.Pos<=20.0f);

 /* ActFromPos clamps out-of-range pos */
 v.Min=0.0; v.Max=100.0;
 Check("ActFromPos(-10) -> 0", ActFromPos(&v,-10.0f)==0);
 Check("ActFromPos(500) -> range", ActFromPos(&v,500.0f)==130);

 (void)p;
 printf(fail?"MATHFAIL\n":"MATHPASS\n");
 return fail;
}
