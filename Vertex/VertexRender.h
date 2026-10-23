/*********************************************/
/*       Vertex Wire Render Header File      */
/*-------------------------------------------*/
/*              NO COPYRIGHT 1995            */
/*-------------------------------------------*/
/*          C.LEGENDRE && S.MAUFROY          */
/*********************************************/

#ifndef __VERTEX_RENDER_H
#define __VERTEX_RENDER_H
#include<stdio.h>
#include<math.h>
#include<X11/Xlib.h>
#include<X11/IntrinsicP.h>
#include<X11/Shell.h>
#include"VertexStruct.h"
#include"VertexMemory.h"
#define GLOBAL_3D 0
#define PLAN_XY 1
#define PLAN_XZ 2
#define PLAN_YZ 3
#define WIRE_RENDER 0
#define OUTLINE_RENDER 1
#define FLAT_RENDER 2

void VertexRenderOutLine();
void VertexRenderSolid();
void VertexRenderPhong();
void VertexRenderGouraud();
void VertexAnim();

void FillFace8(XImage *,int,int,int,int,int,int,int,int,int);
void FillFace16_24(XImage *,int,int,int,int,int,int,int,int,int);
void RenderOutLine(Drawable,GC,int,int,int);
void RenderSolid(XImage *,int,int,PT3D);
#endif

