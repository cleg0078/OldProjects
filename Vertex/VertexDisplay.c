/********************************************************/
/*                     VERTEX 3D V1.0                   */
/*                      UNIX VERSION                    */
/*------------------------------------------------------*/
/*                    Vertex Display File               */
/*                  NO COPYRIGHT May 1995               */
/*------------------------------------------------------*/
/*  Concept And Design: C.LEGENDRE && S.MAUFROY         */
/********************************************************/
 
#include"VertexDisplay.h"
#include"VertexMemory.h"
#define MABS(x) (x<0.0)?-x:x

extern Window win,rootwindow;
extern Window winXY,winXZ,winYZ,win3D;
extern GC gc;
extern Display *dpy;
extern int win_maxx,win_maxy;  /* The Mono View Window Size */
extern int trimaxx,trimaxy;   /* The TriView Window Size */
extern float maxx,maxy,maxz,minx,miny,minz,screen_dist;
extern int ViewType;
extern SCENE3D global_scene;
extern XSegment *sgl;
extern int tbuff;
extern int TriView;
extern unsigned long White,Black,Pink;

float coeffx,coeffy,coeffz;

void DisplayXY(Drawable dwin,int dmaxx,int dmaxy,OBJ3D *tmp,int RT)
{
 int i,j;
 int *ftmp;
 int t1,t2,t3;
 int x1,x2,x3,y1,y2,y3;
 int xc,yc;
 PT3D *ptmp;

 xc = MABS(minx*(dmaxx/(maxx-minx)));
 yc = MABS(miny*(dmaxy/(maxy-miny)));

 coeffx = dmaxx/(float)(maxx-minx+1);
 if(coeffx <= 0.0)
    coeffx = 1.0;

 coeffy = dmaxy/(float)(maxy-miny+1);
 if(coeffy <= 0.0)
    coeffy = 1.0;

 ftmp = (int *)tmp->FCtable;
 ptmp = tmp->PTtable;

 for(j=i=0;i<tmp->NBFACE; i++)
    {
     t1 = *ftmp++;
     t2 = *ftmp++;
     t3 = *ftmp++;
       
     x1  = (short)(ptmp[t1].X*coeffx+xc); y1 = (short)(yc-ptmp[t1].Y*coeffy);
     x2 = (short)(ptmp[t2].X*coeffx+xc);  y2 = (short)(yc-ptmp[t2].Y*coeffy);
     x3 = (short)(ptmp[t3].X*coeffx+xc);  y3 = (short)(yc-ptmp[t3].Y*coeffy);
     sgl[j].x1 = x1;  sgl[j].y1 = y1;  sgl[j].x2 = x2;  sgl[j++].y2 = y2;
     sgl[j].x1 = x2;  sgl[j].y1 = y2;  sgl[j].x2 = x3;  sgl[j++].y2 = y3;
     sgl[j].x1 = x3;  sgl[j].y1 = y3;  sgl[j].x2 = x1;  sgl[j++].y2 = y1;
  
     if( j >= tbuff )
       {
        XDrawSegments(dpy,dwin,gc,sgl,j);
        j=0;
       }        
    }

 XDrawSegments(dpy,dwin,gc,sgl,j);
}

void DisplayXZ(Drawable dwin,int dmaxx,int dmaxy,OBJ3D *tmp,int RT)
{
 int i,j;
 int *ftmp;
 int t1,t2,t3;
 int x1,x2,x3,y1,y2,y3;
 PT3D *ptmp;
 int xc,yc,zc;

 xc = MABS(minx*(dmaxx/(maxx-minx)));
 zc = MABS(maxz*(dmaxy/(maxz-minz)));

 if( (float)(maxx-minx+1.0) != 0.0 )
    coeffx = dmaxx/(float)(maxx-minx+1.0);

 if(coeffx == 0.0)
    coeffx = 1.0;

 if( (float)(maxz-minz+1.0) != 0.0 )
    coeffy = dmaxy/(float)(maxz-minz+1.0); 
 if(coeffy == 0.0)
    coeffy = 1.0;

 ftmp = (int *)tmp->FCtable;
 ptmp = tmp->PTtable;

 for(j=0,i=tmp->NBFACE; i ; i--)
    {
     t1 = *ftmp++;
     t2 = *ftmp++;
     t3 = *ftmp++;

     x1 = (short)(ptmp[t1].X*coeffx+xc);  y1 = (short)(zc+ptmp[t1].Z*coeffy);
     x2 = (short)(ptmp[t2].X*coeffx+xc); y2 = (short)(zc+ptmp[t2].Z*coeffy);
     x3 = (short)(ptmp[t3].X*coeffx+xc); y3 = (short)(zc+ptmp[t3].Z*coeffy);
     
     sgl[j].x1 = x1;  sgl[j].y1 = y1;  sgl[j].x2 = x2;  sgl[j++].y2 = y2;
     sgl[j].x1 = x2;  sgl[j].y1 = y2;  sgl[j].x2 = x3;  sgl[j++].y2 = y3;      
     sgl[j].x1 = x3;  sgl[j].y1 = y3;  sgl[j].x2 = x1;  sgl[j++].y2 = y1;       

      if( j >= tbuff )
       {
        XDrawSegments(dpy,dwin,gc,sgl,j);
        j=0;
       }

    }

 XDrawSegments(dpy,dwin,gc,sgl,j);
}


void DisplayZY(Drawable dwin,int dmaxx,int dmaxy,OBJ3D *tmp,int RT)
{
 int i,j;
 int *ftmp;
 int t1,t2,t3;
 int x1,x2,x3,y1,y2,y3;
 PT3D *ptmp;
 int xc,yc,zc;

 zc = MABS(minz*(dmaxx/(maxz-minz)));
 yc = MABS(miny*(dmaxy/(maxy-miny)));

 if( (float)(maxz-minz+1.0) != 0.0 )
    coeffx = dmaxx/(float)(maxz-minz+1.0);

 if(coeffx == 0.0)
    coeffx = 1.0;

 if( (float)(maxy-miny+1.0) != 0.0 )
    coeffy = dmaxy/(float)(maxy-miny+1.0);

 if(coeffy == 0.0)
    coeffy = 1.0;

 ftmp = (int *)tmp->FCtable;
 ptmp = tmp->PTtable;

 for(j=0,i=tmp->NBFACE; i ; i--)
    {
     t1 = *ftmp++;
     t2 = *ftmp++;
     t3 = *ftmp++;

     x1 = (ptmp[t1].Z*coeffx+zc);  y1 = (yc-ptmp[t1].Y*coeffy);
     x2 = (ptmp[t2].Z*coeffx+zc);  y2 = (yc-ptmp[t2].Y*coeffy);
     x3 = (ptmp[t3].Z*coeffx+zc);  y3 = (yc-ptmp[t3].Y*coeffy);
     sgl[j].x1 = x1;  sgl[j].y1 = y1;  sgl[j].x2 = x2;  sgl[j++].y2 = y2;
     sgl[j].x1 = x2;  sgl[j].y1 = y2;  sgl[j].x2 = x3;  sgl[j++].y2 = y3;
     sgl[j].x1 = x3;  sgl[j].y1 = y3;  sgl[j].x2 = x1;  sgl[j++].y2 = y1;       

     if( j >= tbuff )
       {
        XDrawSegments(dpy,dwin,gc,sgl,j);
        j=0;
       }
    }

 XDrawSegments(dpy,dwin,gc,sgl,j);
}   


void Display3D(Drawable dwin,int dmaxx,int dmaxy,OBJ3D *tmp,int RT)
{
 int i,j;
 int t1,t2,t3;
 float x1,x2,x3,y1,y2,y3,z1,z2,z3;
 PT3D *ptmp;
 int *ftmp;
 int xc,yc;

 xc = MABS(minx*(dmaxx/(maxx-minx)));
 yc = MABS(miny*(dmaxy/(maxy-miny)));

 ftmp = (int *)tmp->FCtable;
 ptmp = (PT3D *)tmp->PTtable;

 for(j=0,i=tmp->NBFACE; i>0 ; i--)
    {
     t1 = *ftmp++;
     t2 = *ftmp++;
     t3 = *ftmp++;

     x1 = ptmp[t1].X;  y1 = ptmp[t1].Y; z1 = (int)(ptmp[t1].Z-minz);
     x2 = ptmp[t2].X;  y2 = ptmp[t2].Y; z2 = (int)(ptmp[t2].Z-minz);
     x3 = ptmp[t3].X;  y3 = ptmp[t3].Y; z3 = (int)(ptmp[t3].Z-minz);

     if( z1 != 0.0 )
       if( z2 != 0.0 )
          if( z3 != 0.0 )
            {
             sgl[j].x1 = (short)(xc+x1*screen_dist/z1); sgl[j].y1 =(short)(yc-y1*screen_dist/z1);
             sgl[j].x2 = (short)(xc+x2*screen_dist/z2); sgl[j].y2 =(short)(yc-y2*screen_dist/z2);
             sgl[j+1].x1 = sgl[j].x2; sgl[j+1].y1 = sgl[j].y2;
             sgl[j+1].x2 = (short)(xc+x3*screen_dist/z3); sgl[j+1].y2 = (short)(yc-y3*screen_dist/z3);
             sgl[j+2].x1 = sgl[j+1].x2;  sgl[j+2].y1 = sgl[j+1].y2;
             sgl[j+2].x2 = sgl[j].x1;    sgl[j+2].y2 = sgl[j].y1;
             j+=3;
 
             if(j>=tbuff)
               { 
                XDrawSegments(dpy,dwin,gc,sgl,j); 
                j=0;
               } 
            }
    }
   XDrawSegments(dpy,dwin,gc,sgl,j);
}

void DisplayPoint(Drawable dwin,int dmaxx,int dmaxy,OBJ3D *tmp,int view)
{
 int i;
 int x1 = 0,y1 = 0;
 PT3D *ptmp;
 int xc,yc,zc;

 switch( view ) 
       {
        case( PLAN_XY ):
             xc = MABS(minx*(dmaxx/(maxx-minx)));
             yc = MABS(miny*(dmaxy/(maxy-miny)));
             coeffx = dmaxx/(float)(maxx-minx+1);
             if(coeffx <= 0.0)
                coeffx = 1.0;
             coeffy = dmaxy/(float)(maxy-miny+1);
             if(coeffy <= 0.0)
                coeffy = 1.0;
             break;             
        case( PLAN_XZ ):
            xc = MABS(minx*(dmaxx/(maxx-minx)));
            zc = MABS(maxz*(dmaxy/(maxz-minz)));
            if( (float)(maxx-minx+1.0) != 0.0 )
               coeffx = dmaxx/(float)(maxx-minx+1.0);
            if(coeffx == 0.0)
               coeffx = 1.0;
            if( (float)(maxz-minz+1.0) != 0.0 )
               coeffy = dmaxy/(float)(maxz-minz+1.0);
            if(coeffy == 0.0)
               coeffy = 1.0;
            break; 
        case( PLAN_YZ ):
            zc = MABS(minz*(dmaxx/(maxz-minz)));
            yc = MABS(miny*(dmaxy/(maxy-miny)));
            if( (float)(maxz-minz+1.0) != 0.0 )
               coeffx = dmaxx/(float)(maxz-minz+1.0);
            if(coeffx == 0.0)
               coeffx = 1.0;
            coeffy = dmaxy/(float)(maxy-miny+1);
            if(coeffy <= 0.0)
               coeffy = 1.0;
       }

 ptmp = tmp->PTtable;

 XSetForeground(dpy,gc,Pink);

 /* One rectangle per unique vertex: the 1995 loop walked faces and drew
    each shared vertex once per incident face (3-6x redundant X traffic). */
 for(i=0;i<tmp->NBPT3D ; i++)
    {
     switch( view)
           {
            case( PLAN_XY ):
                 x1 = (ptmp[i].X*coeffx+xc);  y1 = (yc-ptmp[i].Y*coeffy);
                 break;
            case( PLAN_XZ ):
                 x1 = (ptmp[i].X*coeffx+xc); y1 = (zc+ptmp[i].Z*coeffy);
                 break;
            case( PLAN_YZ ):
                 x1 = (ptmp[i].Z*coeffx+zc);  y1 = (yc-ptmp[i].Y*coeffy);
           }

     XFillRectangle(dpy,dwin,gc,x1-2,y1-2,4,4);
    }
}

void VertexEDITDisplay(OBJ3D *ob)
{
 int i,x1,y1;
 PT3D *ptmp;
 int xc,yc,zc;

 XClearWindow(dpy,win);
 XSetForeground(dpy,gc,White);

 switch(ViewType)
       {
        case(PLAN_XY):
             DisplayXY(win,win_maxx,win_maxy,ob,FALSE);
             ptmp = ob->PTtable;
             xc = MABS(minx*((win_maxx)/(maxx-minx)));
             yc = MABS(miny*((win_maxy)/(maxy-miny)));

             coeffx= win_maxx/(float)(maxx-minx+1);  
             coeffy= win_maxy/(float)(maxy-miny+1);
 
             XSetForeground(dpy,gc,Pink);
             for(i=ob->NBPT3D-1 ; i>=0 ; i--)
                {
                 x1 = (int)(ptmp[i].X*coeffx+xc);  
                 y1 = (int)(yc-ptmp[i].Y*coeffy);
                 XFillRectangle(dpy,win,gc,x1-2,y1-2,4,4);
                }
            break;
        case(PLAN_XZ):    
             DisplayXZ(win,win_maxx,win_maxy,ob,FALSE); 
             ptmp = ob->PTtable;
             xc = MABS(minx*((win_maxx)/(maxx-minx)));
             yc = MABS(maxz*((win_maxy)/(maxz-minz)));

             coeffx= win_maxx/(float)(maxx-minx+1);
             coeffy= win_maxy/(float)(maxz-minz+1);

             XSetForeground(dpy,gc,Pink);
             for(i=ob->NBPT3D-1 ; i>=0 ; i--)
                {
                 x1 = (int)(ptmp[i].X*coeffx+xc);
                 y1 = (int)(yc+ptmp[i].Z*coeffy);
                 XFillRectangle(dpy,win,gc,x1-2,y1-2,4,4);
                }
             break;
        case(PLAN_YZ):
             DisplayZY(win,win_maxx,win_maxy,ob,FALSE);
             break;
       }
} 

void VertexReDisplay()
{
 TREE_OBJ3D *ltmp;
 
 XSetForeground(dpy,gc,White);

 XClearWindow(dpy,winXY);
 XClearWindow(dpy,winXZ);
 XClearWindow(dpy,winYZ);
 XClearWindow(dpy,win3D);
 XClearWindow(dpy,win);

 for(ltmp = global_scene.Head; ltmp ; ltmp = ltmp->Brother )
    if( TriView == FALSE )
      {
       switch(ViewType)
           {
            case(GLOBAL_3D):   Display3D(win,win_maxx,win_maxy,ltmp->Actual,FALSE); break;
            case(PLAN_XY):     DisplayXY(win,win_maxx,win_maxy,ltmp->Actual,FALSE); break;
            case(PLAN_XZ):     DisplayXZ(win,win_maxx,win_maxy,ltmp->Actual,FALSE); break;
            case(PLAN_YZ):     DisplayZY(win,win_maxx,win_maxy,ltmp->Actual,FALSE); break;
           }
      }
    else
      {
       DisplayXY(winXY,trimaxx,trimaxy,ltmp->Actual,FALSE);
       DisplayXZ(winXZ,trimaxx,trimaxy,ltmp->Actual,FALSE);
       DisplayZY(winYZ,trimaxx,trimaxy,ltmp->Actual,FALSE); 
       Display3D(win3D,trimaxx,trimaxy,ltmp->Actual,FALSE);
      }
} 

void VertexDisplay3D()
{
 if(ViewType != GLOBAL_3D )
   {
    ViewType = GLOBAL_3D;
    XStoreName(dpy,rootwindow,"VERTEX 3D     Actual View: GLOBAL 3D");
    VertexReDisplay();
   }
}

void VertexDisplayXY()
{
 if(ViewType != PLAN_XY)
   {
    ViewType = PLAN_XY;
    XStoreName(dpy,rootwindow,"VERTEX 3D     Actual View: PLANE XY");
    VertexReDisplay();
   }
}

void VertexDisplayXZ()
{
 if(ViewType != PLAN_XZ)
   {
    ViewType = PLAN_XZ;
    XStoreName(dpy,rootwindow,"VERTEX 3D     Actual View: PLANE XZ");
    VertexReDisplay();
   }
}

void VertexDisplayZY()
{
 if(ViewType != PLAN_YZ)
   {
    ViewType = PLAN_YZ;
    XStoreName(dpy,rootwindow,"VERTEX 3D     Actual View: PLANE ZY");
    VertexReDisplay();
   }
}

void VertexRTDisplay(OBJ3D *tmp)
{
 if( TriView == FALSE )
   {
    switch(ViewType)
          {
           case(PLAN_XY):     DisplayXY(win,win_maxx,win_maxy,tmp,TRUE); break;
           case(PLAN_XZ):     DisplayXZ(win,win_maxx,win_maxy,tmp,TRUE); break;
           case(PLAN_YZ):     DisplayZY(win,win_maxx,win_maxy,tmp,TRUE); break;
           case(GLOBAL_3D):   Display3D(win,win_maxx,win_maxy,tmp,TRUE); break;
          }
   }
 else
   {
    DisplayXY(winXY,trimaxx,trimaxy,tmp,TRUE);
    DisplayXZ(winXZ,trimaxx,trimaxy,tmp,TRUE);
    DisplayZY(winYZ,trimaxx,trimaxy,tmp,TRUE); 
    Display3D(win3D,trimaxx,trimaxy,tmp,TRUE);
   }
} 

void VertexTriView()
{
 if( TriView == FALSE )
   {
    TriView = TRUE;
    XUnmapWindow(dpy,win);
    XMapWindow(dpy,winXY);
    XMapWindow(dpy,winXZ);
    XMapWindow(dpy,winYZ);
    XMapWindow(dpy,win3D);
   }
 else
   {
    TriView = FALSE;
    XUnmapWindow(dpy,winXY);
    XUnmapWindow(dpy,winXZ);
    XUnmapWindow(dpy,winYZ);
    XUnmapWindow(dpy,win3D);
    XMapWindow(dpy,win);
   }
 
 VertexReDisplay();
}


void DrawLine3D(PT3D p1,PT3D p2)
{
 float x1,y1,z1,x2,y2,z2;
 int px1,py1,px2,py2,lxc,lyc;
 int xc,yc,zc;

 lxc = MABS(minx*(win_maxx/(maxx-minx)));
 lyc = MABS(miny*(win_maxy/(maxy-miny)));

 x1 = p1.X;  y1 = p1.Y; z1 = (int)(p1.Z-minz);
 x2 = p2.X;  y2 = p2.Y; z2 = (int)(p2.Z-minz);

 if( z1 != 0.0 )
    if( z2 != 0.0 )
      {
       px1 = (short)(lxc+x1*screen_dist/z1);
       py1 = (short)(lyc-y1*screen_dist/z1);
       px2 = (short)(lxc+x2*screen_dist/z2);
       py2 = (short)(lyc-y2*screen_dist/z2);
       XSetForeground(dpy,gc,White);
       XDrawLine(dpy,win,gc,px1,py1,px2,py2);
      }
}


void VertexDisplayNormal()
{
 TREE_OBJ3D *ltmp;
 OBJ3D *ob;
 PT3D gc,dir;
 PT3D norm,p1,p2,p3,ptab[3];
 float length;
 int i,nbf;
 int *ftmp;

 for(ltmp = global_scene.Head; ltmp ; ltmp = ltmp->Brother )
    {
     ob = ltmp->Actual;
     ftmp = (int *)ob->FCtable;
     nbf = ob->NBFACE;

     for(i=0; i<nbf ; i++ )
        {
         p1 = ob->PTtable[*ftmp++]; 
         p2 = ob->PTtable[*ftmp++];
         p3 = ob->PTtable[*ftmp++];
        
         gc.X = ( p1.X + p2.X + p3.X) / 3.0;
         gc.Y = ( p1.Y + p2.Y + p3.Y) / 3.0;
         gc.Z = ( p1.Z + p2.Z + p3.Z) / 3.0;
       
         ptab[0]=p1; ptab[1]=p2; ptab[2]=p3;

         norm = GetNormal(ptab);
         length = sqrt(norm.X*norm.X + norm.Y*norm.Y + norm.Z*norm.Z);

         if(length != 0.0)
           {
            norm.X /=length;
            norm.Y /=length;
            norm.Z /=length;
           }

         dir.X = gc.X + norm.X*80.0;
         dir.Y = gc.Y + norm.Y*80.0;
         dir.Z = gc.Z + norm.Z*80.0;
 
         DrawLine3D(gc,dir);
        }
   }
}
 
