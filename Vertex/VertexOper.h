/*********************************************/
/*       Vertex Oiperation Header File       */
/*-------------------------------------------*/
/*              NO COPYRIGHT 1995            */
/*-------------------------------------------*/
/*          C.LEGENDRE && S.MAUFROY          */
/*********************************************/
#ifndef __VERTEX_OPER_H
#define __VERTEX_OPER_H
#include"VertexStruct.h"
#include"VertexOBJ.h"
#include"VertexDisplay.h"
#define GLOBAL_3D 0
#define PLAN_XY 1
#define PLAN_XZ 2
#define PLAN_YZ 3

void VertexZoom();
void VertexMove();
void VertexRotate();
void VertexDuplicate();
void VertexFreeOper();
void VertexRTOper(void fct(float *,int,float,float,float),int oper);

void ZoomPT3D(float * ,int ,float ,float ,float);
void RotatePT3D(float * ,int ,float ,float ,float);
void MovePT3D(float * ,int ,float ,float ,float);
#endif
