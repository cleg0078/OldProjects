/*********************************************/
/*             Vertex Main Include           */
/*-------------------------------------------*/
/*              NO COPYRIGHT 1995            */
/*-------------------------------------------*/
/*          C.LEGENDRE && S.MAUFROY          */
/*********************************************/
#ifndef __VERTEX_H
#define __VERTEX_H
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<X11/Xlib.h>
#include<X11/IntrinsicP.h>
#include<X11/Shell.h>
#include<StringDefs.h>
#include"VertexStruct.h"
#include"VertexGUI.h"
#include"VertexMemory.h"
#include"VertexOBJ.h"
#include"VertexDisplay.h"
#include"VRay.h"

/* Global GUI Gadget */
XGUI gui;
XShell *toplevel;
XWidget *msg,*objlist;

/* Global X11 Structures */
Window win,rootwindow,iconwindow,RenderWin;
Window win3D,winXY,winXZ,winYZ;
GC gc,RenderGC;
Display *dpy;
XGCValues gcv;
XEvent ev;
Colormap cmap;
int bpp;
unsigned long color[216];

/* Global Vertex Data */
SCENE3D global_scene;
float minx,maxx,miny,maxy,minz,maxz; /* World Size */
float screen_dist;  /* Distance Between The Screen And The Observer */
int win_maxx,win_maxy,ed_color;
int trimaxx,trimaxy;
int ViewType,TriView;
int nbobj,nbdisk,nbcube,nbpyra,nbsphere,nbrevol,nbmount,nbcyl,nbtorus,nbcone;
int nbsurf;
PT3D tab[300];
int nbpt,tbuff;
FCList *fclist;
PTList *ptlist;
XSegment *sgl;

/* Global Display Color */
unsigned long Black,White,Gray1,Gray2,Pink;
unsigned long (*GetRGB24)(int,int,unsigned char,unsigned char,unsigned char);
void (*VertexFillFace)(XImage *,int,int,int,int,int,int,int,int,int);

#endif

