/*********************************************/
/*          Vertex Objet Header File         */
/*-------------------------------------------*/
/*              NO COPYRIGHT 1995            */
/*-------------------------------------------*/
/*          C.LEGENDRE && S.MAUFROY          */
/*********************************************/
#ifndef __VERTEX_OBJ_H
#define __VERTEX_OBJ_H
#include<math.h>
#include<X11/Xlib.h>
#include<X11/Xutil.h>
#include<X11/IntrinsicP.h>
#include<X11/Shell.h>
#include<StringDefs.h>
#include"VertexStruct.h"
#include"VertexMemory.h"
#include"VertexDisplay.h"
#define GLOBAL_3D 0
#define PLAN_XY 1
#define PLAN_XZ 2
#define PLAN_YZ 3

#ifndef PI
#define PI M_PI
#endif

float VertexATOF(char *);
void VertexCreateDisk();
void VertexCreateCube();
void VertexCreatePyramid();
void VertexCreateSphere();
void VertexCreateHSphere();
void VertexCreateRevol(); 
void VertexCreateCylinder();
void VertexCreateCone();
void VertexCreateSurface();
void VertexCreateTorus();

#endif
