/*********************************************/
/*         Vertex Display Header File        */
/*-------------------------------------------*/
/*              NO COPYRIGHT 1995            */
/*-------------------------------------------*/
/*          C.LEGENDRE && S.MAUFROY          */
/*********************************************/
#ifndef __VERTEX_DISP_H
#define __VERTEX_DISP_H
#include<math.h>
#include<stdio.h>
#include<X11/Xlib.h>
#include<X11/IntrinsicP.h>
#include<X11/Shell.h>
#include"VertexStruct.h"
#include"VertexMemory.h"
#define GLOBAL_3D 0
#define PLAN_XY 1
#define PLAN_XZ 2
#define PLAN_YZ 3

void VertexDisplay3D();
void VertexDisplayXY();
void VertexDisplayXZ();
void VertexDisplayZY();
void VertexReDisplay();
void VertexEDITDisplay(OBJ3D *);
void VertexRTDisplay(OBJ3D *);
void VertexTriView();
void DisplayXY(Drawable,int,int,OBJ3D *,int);
void DisplayXZ(Drawable,int,int,OBJ3D *,int);
void DisplayZY(Drawable,int,int,OBJ3D *,int);
void Display3D(Drawable,int,int,OBJ3D *,int);
void DisplayPoint(Drawable,int,int,OBJ3D *,int);

void VertexDisplayNormal();
#endif

