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
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#define LABS(x) (x<0?-x:x)


typedef struct SphereREC
{
 float CenterX,CenterY,CenterZ;
 float Radius,Radius2;
}SPHEREREC;

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


float PDotProduct(PT3D v1,PT3D v2)
{
 static float temp;

 if((v1.X == 0.0 && v1.Y == 0.0))
    return(acos(1.0)*180);

 temp = sqrt(v1.X * v1.X + v1.Y * v1.Y + v1.Z * v1.Z);
 if(temp != 0.0)
   {
    v1.X = v1.X / temp;
    v1.Y = v1.Y / temp;
    v1.Z = v1.Z / temp;
   }

 temp = sqrt(v2.X * v2.X + v2.Y * v2.Y + v2.Z * v2.Z);
 if(temp != 0.0 )
   {
    v2.X = v2.X / temp;
    v2.Y = v2.Y / temp;
    v2.Z = v2.Z / temp;  
   }

 temp = v1.X * v2.X + v1.Y * v2.Y +v1.Z*v2.Z ;
 
 if(temp > 1.0)
    temp = fmod(temp, 1.0);

 if(temp < -1.0)
    temp = -fmod(-temp, 1.0);

 return(acos(temp)/PI*180.0);
}

void Render(Drawable rdw,GC rgc,int mx,int my,int mc,PT3D Eye,SPHEREREC sph)
{
 int x,y,color;
 PT3D Normal;
 float Angle;
 float X,Y,Z,Z1;
 float Radius,Radius2;
 float CenterX,CenterY,CenterZ;
 
/*
 XClearWindow(dpy,rdw);
*/

 CenterX = sph.CenterX;
 CenterY = sph.CenterY;
 CenterZ = sph.CenterZ;
 Radius = sph.Radius;
 Radius2 = sph.Radius2;

 for(y=0; y<=my ; y++ )
    {
     Y = maxy-y*(maxy-miny)/(float)my;
     for(x=0 ; x<=mx ; x++ )
        {  
         X = minx+x*(maxx-minx)/(float)mx;
         Z1 = Radius2 - (X-CenterX)*(X-CenterX) - (Y-CenterY)*(Y-CenterY);
 
         if( Z1 >= 0.0 )    
           {
            Z = -sqrt(Z1)+CenterZ;
            #ifdef __DEBUG
            printf("X=%f Y=%f Z=%f\n",X,Y,Z);
            #endif

            Normal.X = 2*(X-CenterX)*cos(cos(X));
            Normal.Y = 2*(Y-CenterY);
            Normal.Z = 2*(Z-CenterZ)*cos(sin(X));
            color = 0;

            Angle = PDotProduct(Normal,Eye);
            if( Angle >= 90.0 || Angle <= 0.0)
               color = 0;
            else
               color = (90.0-Angle)*mc/90.0;

            XSetForeground(dpy,rgc,color);
            XFillRectangle(dpy,rdw,rgc,x,y,1,1);
           }
        }
    }

 usleep(0L);

}
         

void RenderPhong()
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
 int inc,max,i,cont=1,t0,t1;
 int img,rmaxx,rmaxy;
 float ang;
 PT3D Eye;
 SPHEREREC sph;

 XSetWindowAttributes attrs;

 rmaxx = 300;
 rmaxy = 300;
 Win = XCreateSimpleWindow(gui.dpy,DefaultRootWindow(gui.dpy),
                           0,0,rmaxx,rmaxy,1,0,0);

 gcv.function = GXcopy;
 gcv.line_width = 1;
 lgc = XCreateGC(gui.dpy,Win,GCFunction|GCLineWidth,&gcv);
 XSelectInput(gui.dpy,Win,KeyPress|KeyRelease);
 
 cmap=XCreateColormap(dpy,Win,DefaultVisual(dpy,DefaultScreen(dpy)),AllocAll);
 max = DisplayPlanes(gui.dpy,DefaultScreen(gui.dpy));
 max = 1<<max;
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

 t0=time(0);

 sph.CenterX = sph.CenterY = sph.CenterZ = 0.0;
 sph.Radius = 300.0;
 sph.Radius2 = 300.0 * 300.0;

 for(img=0, ang=0.0 ; ang <=5.0*PI ; ang+= PI/15 )
    {
      img++;
      Eye.X =  400.0  - 800 *cos(ang)*sin(ang);
      Eye.Y =  300 - 600.0 * sin(ang); 
      Eye.Z = -600.0 + 1200.0 * cos(ang); 
/*
      sph.CenterX = 0.0;
      sph.CenterY = 300.0 - 600.0 *sin(ang);
*/
            
      Render(Win,lgc,rmaxx,rmaxy,max,Eye,sph);
    }
 
 t1=time(0);

 printf("%d Images In %d s\n",img,t1-t0);
 
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


void VertexRenderPhong()
{
 RenderPhong();
}


