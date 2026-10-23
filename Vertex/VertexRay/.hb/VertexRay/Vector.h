/******************************************/
/*                Vertex 3D               */
/*            Ray Tracing Module          */
/*               Version 1.0              */
/*----------------------------------------*/
/*       Vector Operations Prototypes     */
/*----------------------------------------*/
/* Concept And Design:C.LEGENDRE Nov 1995 */
/******************************************/
#ifndef __VRVECTOR_H
#define __VRVECTOR_H
#include"VRStruct.h"

VECTOR vadd(register VECTOR,register VECTOR);
VECTOR vsub(register VECTOR,register VECTOR);
VECTOR vneg(register VECTOR);
VECTOR svproduct(register VDBL,register VECTOR);
VDBL   vdot(register VECTOR,register VECTOR);
VECTOR vcross(register VECTOR,register VECTOR);
VECTOR norm(register VECTOR);

#endif
