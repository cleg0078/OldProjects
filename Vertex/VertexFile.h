/*********************************************/
/*             Vertex FILE Include           */
/*-------------------------------------------*/
/*              NO COPYRIGHT 1995            */
/*-------------------------------------------*/
/*          C.LEGENDRE && S.MAUFROY          */
/*********************************************/

#ifndef __VERTEX_FILE_H
#define __VERTEX_FILE_H
#include<stdio.h>
#include<stdlib.h>
#include<strings.h>
#include<dirent.h>
#include<sys/stat.h>
#include<sys/utsname.h>
#include<X11/Xlib.h>
#include<X11/IntrinsicP.h>
#include<X11/Shell.h>
#include<StringDefs.h>
#include"VertexStruct.h"
#include"VertexMemory.h"

/* Function prototype */
extern void VertexLoad();
extern void VertexSave();
extern void VertexSaveASCII();
extern void VertexSavePOV();
extern void VertexSave3DS();
extern void VertexFileGEN();
extern int BadFaces(OBJ3D *ob);
#endif
