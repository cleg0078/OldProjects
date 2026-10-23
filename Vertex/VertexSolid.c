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
extern int tbuff;
extern XWidget *msg;
extern XGUI gui;
extern unsigned long White,Black;
extern unsigned long color[216];
extern unsigned long (*GetRGB24)(int,int,unsigned char,unsigned char,unsigned char);

int SCompWire(const void *pt1,const void *pt2)
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
  
/* Per-frame WireInfo arrays: animation re-renders every frame, so recycle
   the buffers in a small pool instead of calloc/free per object per frame.
   All max entries are written by the loop below before qsort reads them,
   so a reused (dirty) buffer is safe. */
static struct { WireInfo *p; int cap; } s_wipool[64];
static int s_nwipool;

static WireInfo *SWireInfoGet(int nb)
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

static void SWireInfoPut(WireInfo *p,int cap)
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

int SBuildWireInfo(WireRenderInfo *w,OBJ3D *ob)
{
 int i,max=ob->NBFACE;
 WireInfo *wi;
 PT3D *ptmp;
 FACE3D *ftmp;
 float gravz,obx,oby,obz;


 w->Next = 0;
 w->Winf = SWireInfoGet(max);
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
   
   qsort(w->Winf,max,sizeof(WireInfo),SCompWire);
   return(1);
} 


int SRenderObject(WireRenderInfo *wd,OBJ3D **ob,WireRenderInfo **wa)
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

#include"triangle2.c"

void RenderSolid(XImage *im,int mx,int my,PT3D Light)
{
 TREE_OBJ3D *ltmp;
 int i,nb,face;
 int t1,t2,t3;
 float x1,x2,x3,y1,y2,y3,z1,z2,z3;
 PT3D *ptmp,Normal,tabt[3];
 int yc,xc;           /* Center of the output window */
 FACE3D *ftmp;
 WireRenderInfo *wri,*wtmp1,*wtmp2,*wrend;
 XPoint pt[4];
 OBJ3D *obtmp;
 RGBColor *ctmp;
 float L,Angle,llen;
 int rcolor,gcolor,bcolor;

 xc = mx/2;
 yc = my/2;
 llen = Length(Light);   /* loop-invariant: |Light| once per render */

 ltmp = global_scene.Head;
 if(ltmp == 0 )
    return;

 ltmp = global_scene.Head;
 wri = (WireRenderInfo *)calloc(sizeof(WireRenderInfo),1);
 if(wri == NULL)
   {
    puts("No memory For Wire Render");
    return;
   }

 ftmp = (FACE3D *)(ltmp->Actual)->FCtable;
 ptmp = (PT3D *)(ltmp->Actual)->PTtable;

 if( SBuildWireInfo(wri,ltmp->Actual) == 0)
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
        SWireInfoPut(wri->Winf, wri->Obj ? wri->Obj->NBFACE : 0);
        free(wri);
        return;
      }

     wtmp2->Next = wtmp1;
     if( SBuildWireInfo(wtmp1,ltmp->Actual) == 0 )
       {
        puts("No memory For Wire Render");
        SWireInfoPut(wri->Winf, wri->Obj ? wri->Obj->NBFACE : 0);
        free(wri);
        return;
       }
    }

 nb=0;
 while( SRenderObject(wri,&obtmp,&wrend) )
        {
         face = wrend->Winf[(wrend->start)++].ind;

         ftmp = (FACE3D *)(obtmp->FCtable);
         ptmp = (PT3D *)(obtmp->PTtable);
         ctmp = (RGBColor *)(obtmp->Color);
         t1 = ftmp[face].I1;
         t2 = ftmp[face].I2;
         t3 = ftmp[face].I3;
         
         z1 = (ptmp[t1].Z-minz);
         z2 = (ptmp[t2].Z-minz);
         z3 = (ptmp[t3].Z-minz);

         if( z1 != 0 )
           if( z2 != 0 )
              if( z3 != 0 )
                {
                 tabt[0] = ptmp[t1];
                 tabt[1] = ptmp[t2];
                 tabt[2] = ptmp[t3];
                 Normal = GetNormal(tabt);
                 Angle = Light.X*Normal.X+Light.Y*Normal.Y+Light.Z*Normal.Z; 

                 if(Angle <= 0.0 )
                    Angle *=-1;
                  
                 x1 = (ptmp[t1].X);  y1 = (ptmp[t1].Y);
                 x2 = (ptmp[t2].X);  y2 = (ptmp[t2].Y);
                 x3 = (ptmp[t3].X);  y3 = (ptmp[t3].Y);
                 pt[0].x = (int)ceil(xc+x1*screen_dist/z1); pt[0].y =(int)ceil(yc-y1*screen_dist/z1);
                 pt[1].x = (int)ceil(xc+x2*screen_dist/z2); pt[1].y = (int)ceil(yc-y2*screen_dist/z2);
                 pt[2].x = (int)ceil(xc+x3*screen_dist/z3); pt[2].y = (int)ceil(yc-y3*screen_dist/z3);
                 pt[3].x = pt[0].x; pt[3].y = pt[0].y;
 
                 L = llen*Length(Normal);
  
                 if( L > 0.0 )  
                    Angle /= L;
                 else         
                    Angle = 0.0;
                 if( Angle > 0.0 ) 
                   {
                    rcolor= ctmp[face].Red * Angle;
                    gcolor= ctmp[face].Green * Angle;
                    bcolor= ctmp[face].Blue * Angle;
                   }
                 else
                   rcolor = gcolor = bcolor = 0; 
                 
                 VertexFillFace(im,pt[0].x,pt[0].y,pt[1].x,pt[1].y,pt[2].x,pt[2].y,rcolor,gcolor,bcolor);
                }
        }
  
 for(wtmp1=wri ; wtmp1 ; wtmp1=wtmp2)
    {
     wtmp2 = wtmp1->Next;
     SWireInfoPut(wtmp1->Winf, wtmp1->Obj ? wtmp1->Obj->NBFACE : 0);
     free( wtmp1 );
    }
}


void RenderSol()
{
 Window LWin;
 XSetWindowAttributes attrs;
 XComposeStatus compose;
 XImage *im;
 XGCValues gcv;
 XColor color[256];
 XEvent ev;
 char test[2];
 KeySym key;
 int inc,max,i,cont=1;
 int pad,rmaxx,rmaxy;
 PT3D Light;
 char *data;

 rmaxx = win_maxx;
 rmaxy = win_maxy;


 LWin = XCreateSimpleWindow(gui.dpy,DefaultRootWindow(gui.dpy),
                           0,0,win_maxx,win_maxy,1,White,Black);
 XSelectInput(gui.dpy,LWin,KeyPress|KeyRelease);

  
 attrs.backing_store = Always;
 XChangeWindowAttributes(gui.dpy,LWin,CWBackingStore,&attrs);
 XMapWindow(gui.dpy,LWin);
 XFlush(dpy);

 switch( DisplayPlanes(gui.dpy,DefaultScreen(gui.dpy)) )
        {
         case(8):  data = (unsigned char *)calloc(win_maxx*win_maxy,1);
                   memset(data,Black,win_maxx*win_maxy);
                   pad=8;
                   break;
         case(15):
         case(16): data = (unsigned char *)calloc(2*win_maxx*win_maxy,1);
                   pad=16;
                   break;
         case(24): data = (unsigned char *)calloc(4*win_maxx*win_maxy,1);
                   pad = 32;
        }

 im=NULL;
 im = XCreateImage(gui.dpy, DefaultVisual(gui.dpy,DefaultScreen(gui.dpy)),
                   DisplayPlanes(gui.dpy,DefaultScreen(gui.dpy)),
                   ZPixmap, 0, data,win_maxx,win_maxy,pad,win_maxx*(pad/8));

 Light.X = 0.0;
 Light.Y = 0.0;
 Light.Z = minz;

 XClearWindow(dpy,LWin);
 RenderSolid(im,rmaxx,rmaxy,Light);
 XPutImage(dpy,LWin,gc,im,0,0,0,0,win_maxx,win_maxy);

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

 SetTextString(msg,"Quit Solid Render");
 XDestroyWindow(gui.dpy,LWin);
 VertexReDisplay();
}


void VertexRenderSolid()
{
 RenderSol();
}

