/********************************************************/
/*                     VERTEX 3D V1.0                   */
/*                      UNIX VERSION                    */
/*------------------------------------------------------*/
/*                 Vertex Wire Render File              */
/*                  NO COPYRIGHT May 1995               */
/*------------------------------------------------------*/
/*  Concept And Design: C.LEGENDRE && S.MAUFROY         */
/********************************************************/
 
#include "VertexRender.h"
#include "VertexMemory.h"
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#define LABS(x) (x<0?-x:x)
#define max3(x,y,z) ((x>y)?((x>z)?1:3):((y>z)?2:3))
#define Length(a) sqrt(a.X*a.X + a.Y*a.Y + a.Z*a.Z)

extern Window win;
extern Window win3D;
extern GC gc;
extern Display *dpy;
extern int win_maxx,win_maxy; 
extern float maxx,maxy,maxz,minx,miny,minz,screen_dist;
extern int ViewType,TriView;
extern int trimaxx,trimaxy;
extern SCENE3D global_scene;
extern XSegment *sgl;
extern int tbuff;
extern XWidget *msg;
extern XGUI gui;

int GCompWire(const void *pt1,const void *pt2)
{
 WireInfo *p1,*p2;

 p1 = (WireInfo *)pt1;
 p2 = (WireInfo *)pt2;

 if( p1->Distance < p2->Distance)
    return(1);
 
 if( p1->Distance == p2->Distance)
    return(0);

 if( p1->Distance > p2->Distance)
    return(-1);

 return(0);
}
  
/* Per-frame WireInfo arrays: recycle buffers in a small pool instead of
   calloc/free per object per frame. All max entries are written before the
   qsort reads them, so a reused (dirty) buffer is safe. */
static struct { WireInfo *p; int cap; } s_wipool[64];
static int s_nwipool;

static WireInfo *GWireInfoGet(int nb)
{
 int i;
 for(i=0 ; i<s_nwipool ; i++)
    if(s_wipool[i].cap >= nb)
       {
        WireInfo *p = s_wipool[i].p;
        s_wipool[i] = s_wipool[--s_nwipool];   /* remove slot (swap w/ last) */
        return(p);
       }
 return((WireInfo *)calloc(nb*sizeof(WireInfo),1));
}

static void GWireInfoPut(WireInfo *p,int cap)
{
 if(p == NULL)
    return;
 if(s_nwipool < 64)
   {
    s_wipool[s_nwipool].p = p;
    s_wipool[s_nwipool].cap = cap;
    s_nwipool++;
   }
 else
    free(p);
}

int GBuildWireInfo(WireRenderInfo *w,OBJ3D *ob)
{
 int i,max=ob->NBFACE;
 WireInfo *wi;
 PT3D *ptmp;
 FACE3D *ftmp;
 float gravz,obx,oby,obz;


 w->Next = 0;
 w->Winf = GWireInfoGet(max);
 if(w->Winf == NULL )
    return(0);

 w->Obj = ob;
 w->start = 0;
 w->end = max;
 
 obx = 0.0;
 oby = 0.0;
 obz = minz;
 ptmp = ob->PTtable;
 ftmp = ob->FCtable;

 for(wi=(w->Winf),i=0; i<max ; i++)
    {
 /*    gravx = (ptmp[ftmp[i].I1].X+ptmp[ftmp[i].I2].X+ptmp[ftmp[i].I3].X)/3.0;
     gravy = (ptmp[ftmp[i].I1].Y+ptmp[ftmp[i].I2].Y+ptmp[ftmp[i].I3].Y)/3.0;
 */
    gravz = (ptmp[ftmp[i].I1].Z+ptmp[ftmp[i].I2].Z+ptmp[ftmp[i].I3].Z)/3.0;
 
/*   
     gravx = fabs(obx)-fabs(gravx);
     gravy = fabs(oby)-fabs(gravy);
*/ 
     gravz -= obz;

     wi[i].Distance = fabs(gravz);
     wi[i].ind = i;
   }
   
   qsort(w->Winf,max,sizeof(WireInfo),GCompWire);
   return(1);
} 


int GRenderObject(WireRenderInfo *wd,OBJ3D **ob,WireRenderInfo **wa)
{
 WireRenderInfo *wtmp;
 int i,act=0;
 float maxd=-1.0;

 *ob=NULL;

 for(i=0,*wa=wtmp=wd ; wtmp ; wtmp=wtmp->Next,i++ )
     if(wtmp->start < wtmp->end)
         if( wtmp->Winf[wtmp->start].Distance > maxd )
         {
           act= 1;
           maxd = wtmp->Winf[wtmp->start].Distance;
           *ob = wtmp->Obj;
           *wa =wtmp;
         }
 
 return(act);
}


 

void RenderGouraud(Drawable rdw,GC rgc,int mx,int my,int mc,PT3D Eye)
{
 TREE_OBJ3D *ltmp;
 int i,nb,face;
 int t1,t2,t3;
 int x1,x2,x3,y1,y2,y3,z1,z2,z3;
 PT3D *ptmp,Normal,pttmp[3];
 int yc,xc;           /* Center of the output window */
 FACE3D *ftmp;
 WireRenderInfo *wri,*wtmp1,*wtmp2,*wrend;
 XPoint pt[4];
 OBJ3D *obtmp;
 float Angle,elen;
 int color;

 xc = mx/2;
 yc = my/2;
 elen = Length(Eye);   /* loop-invariant: |Eye| once per render */

 ltmp = global_scene.Head; 
 if(ltmp == 0 )
    return;


 for(ltmp=global_scene.Head ; ltmp ; ltmp=ltmp->Brother)
     if(ltmp->Actual->Unified == 0)
       {
        PT3D refpt = { 0.0, maxy, minz };   /* same reference the menu callback uses */
        UnifyObject(ltmp->Actual,refpt);
       }

 ltmp = global_scene.Head;
 wri = (WireRenderInfo *)calloc(sizeof(WireRenderInfo),1);
 if(wri == NULL)
   {
    puts("No memory For Wire Render");
    return;
   }

 ftmp = (FACE3D *)(ltmp->Actual)->FCtable;
 ptmp = (PT3D *)(ltmp->Actual)->PTtable;

 if( GBuildWireInfo(wri,ltmp->Actual) == 0)
   {
    puts("No memory For Wire Render");
    free(wri);
    return;
   }

 for(i=0,ltmp=ltmp->Brother; ltmp ; ltmp=ltmp->Brother,i++)
    {
     for(wtmp2=wtmp1=wri; wtmp1 ; wtmp2=wtmp1,wtmp1=wtmp1->Next );
     wtmp1= (WireRenderInfo *)calloc(sizeof(WireRenderInfo),1);
     if(wtmp1 == NULL)
       {
        puts("No memory For Wire Render");
        GWireInfoPut(wri->Winf, wri->Obj ? wri->Obj->NBFACE : 0);
        free(wri);
        return;
      }

     wtmp2->Next = wtmp1;
     if( GBuildWireInfo(wtmp1,ltmp->Actual) == 0 )
       {
        puts("No memory For Wire Render");
        GWireInfoPut(wri->Winf, wri->Obj ? wri->Obj->NBFACE : 0);
        free(wri);
        return;
       }
    }


 XClearWindow(dpy,rdw);
 nb=0;
 while( GRenderObject(wri,&obtmp,&wrend) )
        {
         face = wrend->Winf[(wrend->start)++].ind;

         ftmp = (FACE3D *)(obtmp->FCtable);
         ptmp = (PT3D *)(obtmp->PTtable);

         t1 = ftmp[face].I1;
         t2 = ftmp[face].I2;
         t3 = ftmp[face].I3;
         
         x1 = ptmp[t1].X;  y1 = ptmp[t1].Y; z1 = (int)(ptmp[t1].Z-minz);
         x2 = ptmp[t2].X;  y2 = ptmp[t2].Y; z2 = (int)(ptmp[t2].Z-minz);
         x3 = ptmp[t3].X;  y3 = ptmp[t3].Y; z3 = (int)(ptmp[t3].Z-minz);

         if( z1 != 0 )
           if( z2 != 0 )
              if( z3 != 0 )
                {
                 pt[0].x = (int)(xc+x1*screen_dist/z1);  pt[0].y =(int)(yc-y1*screen_dist/z1);
                 pt[1].x = (int)(xc+x2*screen_dist/z2);  pt[1].y =(int)(yc-y2*screen_dist/z2);
                 pt[2].x = (int)(xc+x3*screen_dist/z3);  pt[2].y = (int)(yc-y3*screen_dist/z3);
                 pt[3].x = pt[0].x;                      pt[3].y = pt[0].y;                         
           
                 pttmp[0]= ptmp[t1]; pttmp[1]= ptmp[t2]; pttmp[2]= ptmp[t3];
                 Normal = GetNormal(pttmp);
                       
                 color = (int)16*(Normal.X*Eye.X+Normal.Y*Eye.Y+Normal.Z*Eye.Z)
                         /(Length(Normal)*elen+1.0);
   
                 Angle = DotProduct(Normal,Eye);
                 
                 if( Angle <= 90.0 )    
                    color = (int)(90.0-Angle)*mc/90.0;
                 else
                    color = 0;
           
                 XSetForeground(gui.dpy,rgc,color);
                 XFillPolygon(gui.dpy,rdw,rgc,pt,4,Convex,CoordModeOrigin);
                }
        }
  
 for(wtmp1=wri ; wtmp1 ; wtmp1=wtmp2)
    {
     wtmp2 = wtmp1->Next;
     GWireInfoPut(wtmp1->Winf, wtmp1->Obj ? wtmp1->Obj->NBFACE : 0);
     free( wtmp1 );
    }
}


void RenderGour()
{
 Window Win;
 GC lgc;
 XGCValues gcv;
 XColor color[256];
 XEvent ev;
 char test[2];
 XComposeStatus compose;
 KeySym key;
 Colormap cmap;
 int inc,max,i,cont=1;
 int rmaxx,rmaxy;
 PT3D Eye;
 XSetWindowAttributes attrs;

 rmaxx = win_maxx;
 rmaxy = win_maxy;
 Win = XCreateSimpleWindow(gui.dpy,DefaultRootWindow(gui.dpy),
                           0,0,win_maxx,win_maxy,1,0,0);

 gcv.function = GXcopy;
 lgc = XCreateGC(gui.dpy,Win,GCFunction,&gcv);
 XSelectInput(gui.dpy,Win,KeyPress|KeyRelease);
 
 cmap=XCreateColormap(dpy,Win,DefaultVisual(dpy,DefaultScreen(dpy)),AllocAll);
 max = DisplayPlanes(gui.dpy,DefaultScreen(gui.dpy));
 max = 1<<max;
 if( max > 256 )
    max = 256;   /* color[256]: on a 16/24-bit screen only a 256-entry
                    grayscale ramp is installed (fixed 2026) */
 inc = 65536/(max);
 for(i=0;i<max;i++)
    {
     color[i].pixel = i;
     color[i].flags = DoRed|DoGreen|DoBlue;
     color[i].red = color[i].green = color[i].blue = i*inc;
    }

 XStoreColors(dpy,cmap,color,max);
 attrs.colormap = cmap;
 attrs.backing_store = Always;
 XChangeWindowAttributes(gui.dpy,Win,CWBackingStore|CWColormap,&attrs);
 XMapWindow(gui.dpy,Win);
 XFlush(dpy);

 Eye.X = 0.0;
 Eye.Y = 0.0;
 Eye.Z = maxz;
   
 RenderGouraud(Win,lgc,rmaxx,rmaxy,max,Eye);

 SetTextString(msg,"Press Space Bar To Continue");
 while(cont)
      {
        XNextEvent(gui.dpy,&ev); 
        if(ev.type ==KeyRelease)
          {
           XLookupString((XKeyEvent *)&ev,test,1,&key,&compose);
           if( test[0] == ' ' )
              cont=0;
          }
       }

 SetTextString(msg,"Quit Render Mode");

 XFreeGC(gui.dpy,lgc);
 XDestroyWindow(gui.dpy,Win);
 XFreeColormap(gui.dpy,cmap);
 VertexReDisplay();
}


void VertexRenderGouraud()
{
 RenderGour();
}

