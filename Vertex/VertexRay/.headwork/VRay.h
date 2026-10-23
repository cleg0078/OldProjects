/******************************************/
/*                Vertex 3D               */
/*            Ray Tracing Module          */
/*               Version 1.0              */
/*----------------------------------------*/
/* Concept And Design:C.LEGENDRE Nov 1995 */
/******************************************/

#ifndef __VRAY_H
#define __VRAY_H
#include"VRStruct.h"
#include"Vector.h"
#include"Display.h"
#include"Parse.h"

extern void RayTrace(char *,int);

SCENE scene;
OBJ obj[MAX_OBJ];
LIGHT light[MAX_LIGHT];
int nobj,nosph,nosq;    
int nlight,bgflag;
int display,verbose,xres,yres,maxrec_level;
VDBL fov;
VECTOR eye,eye_dir,up,right;
VDBL time1,time2;

#endif
