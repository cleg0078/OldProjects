/********************************************************/
/*                     VERTEX 3D V1.0                   */
/*                      UNIX VERSION                    */
/*------------------------------------------------------*/
/*               Vertex OutLine Render File             */
/*                  NO COPYRIGHT May 1995               */
/*------------------------------------------------------*/
/*  Concept And Design: C.LEGENDRE && S.MAUFROY         */
/********************************************************/
 
#include "VertexRender.h"
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#define LABS(x) (x<0?-x:x)
#define max3(x,y,z) ((x>y)?((x>z)?1:3):((y>z)?2:3))
#define MAX3(x,y,z) (((x)>(y))?(((x)>(z))?(x):(z)):(((y)>(z))?(y):(z)))
#define MIN3(x,y,z) (((x)<(y))?(((x)<(z))?(x):(z)):(((y)<(z))?(y):(z)))
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

int CompWire(const void *pt1,const void *pt2)
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
  
int BuildWireInfo(WireRenderInfo *w,OBJ3D *ob)
{
 int i,max=ob->NBFACE;
 WireInfo *wi;
 PT3D *ptmp;
 FACE3D *ftmp;
 float gravz,obx,oby,obz;


 w->Next = 0;
 w->Winf = calloc(max*sizeof(WireInfo),1);
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
   
   qsort(w->Winf,max,sizeof(WireInfo),CompWire);
   return(1);
} 


int RenderObject(WireRenderInfo *wd,OBJ3D **ob,WireRenderInfo **wa)
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

void RenderOutLine(Drawable rdw,GC rgc,int mx,int my,int mc)
{
 TREE_OBJ3D *ltmp;
 int i,nb,face;
 int t1,t2,t3;
 int x1,x2,x3,y1,y2,y3,z1,z2,z3;
 PT3D *ptmp;
 int yc,xc;           /* Center of the output window */
 FACE3D *ftmp;
 WireRenderInfo *wri,*wtmp1,*wtmp2,*wrend;
 XPoint pt[4];
 OBJ3D *obtmp;

 xc = mx/2;
 yc = my/2;

 ltmp = global_scene.Head; 
 if(ltmp == 0 )
    return;

 wri = (WireRenderInfo *)calloc(sizeof(WireRenderInfo),1);
 if(wri == NULL)
   {
    SetTextString(msg,"No memory For Wire Render");
    return;
   }

 ftmp = (FACE3D *)(ltmp->Actual)->FCtable;
 ptmp = (PT3D *)(ltmp->Actual)->PTtable;

 if( BuildWireInfo(wri,ltmp->Actual) == 0)
   {
    SetTextString(msg,"No memory For Wire Render");
    free(wri);
    return;
   }

 for(i=0,ltmp=ltmp->Brother; ltmp ; ltmp=ltmp->Brother,i++)
    {
     for(wtmp2=wtmp1=wri; wtmp1 ; wtmp2=wtmp1,wtmp1=wtmp1->Next );
     wtmp1= (WireRenderInfo *)calloc(sizeof(WireRenderInfo),1);
     if(wtmp1 == NULL)
       {
        SetTextString(msg,"No memory For Wire Render");
        free(wri->Winf);
        free(wri);
        return;  
       }

     wtmp2->Next = wtmp1;
     if( BuildWireInfo(wtmp1,ltmp->Actual) == 0 )
       {
        SetTextString(msg,"No memory For Wire Render"); 
        free(wri->Winf);
        free(wri);  
        return;  
       }
    }


 nb=0;
 while( RenderObject(wri,&obtmp,&wrend) )
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
                 
                 XSetForeground(dpy,rgc,BlackPixel(dpy,DefaultScreen(dpy)));
                 XFillPolygon(dpy,rdw,rgc,pt,4,Convex,CoordModeOrigin);
                 XSetForeground(dpy,rgc,WhitePixel(dpy,DefaultScreen(dpy)));
                 XDrawLines(dpy,rdw,rgc,pt,4,CoordModeOrigin);
                }
        }
  
 for(wtmp1=wri ; wtmp1 ; wtmp1=wtmp2)
    {
     wtmp2 = wtmp1->Next;
     free( wtmp1->Winf);
     free( wtmp1 );
    }
}


void RenderOL(BOOL s)
{
 Window Win;
 GC lgc;
 XGCValues gcv;
 XEvent ev;
 char test[2];
 XComposeStatus compose;
 KeySym key;
 int max,cont=1;
 int rmaxx,rmaxy;
 PT3D Eye;
 XSetWindowAttributes attrs;
   
 rmaxx = win_maxx;
 rmaxy = win_maxy;
 Win = XCreateSimpleWindow(gui.dpy,DefaultRootWindow(gui.dpy),
                           0,0,rmaxx,rmaxy,1,0,BlackPixel(dpy,DefaultScreen(dpy)));

 gcv.function = GXcopy;
 lgc = XCreateGC(gui.dpy,Win,GCFunction,&gcv);
 XSelectInput(gui.dpy,Win,KeyPress|KeyRelease);

 attrs.backing_store = Always;
 XChangeWindowAttributes(gui.dpy,Win,CWBackingStore,&attrs);
 XMapWindow(gui.dpy,Win);
 XFlush(dpy);

 Eye.X = 0.0;
 Eye.Y = maxy;
 Eye.Z = 0.0;

 XClearWindow(dpy,Win);
 RenderOutLine(Win,lgc,rmaxx,rmaxy,max);

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
 VertexReDisplay();
}

void VertexRenderOutLine()
{
 RenderOL(FALSE);
}

