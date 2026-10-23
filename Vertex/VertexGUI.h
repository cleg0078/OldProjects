/*********************************************/
/*             Vertex  GUI Include           */
/*-------------------------------------------*/
/*              NO COPYRIGHT 1995            */
/*-------------------------------------------*/
/*          C.LEGENDRE && S.MAUFROY          */
/*********************************************/

#ifndef __VERTEX_GUI_H
#define __VERTEX_GUI_H
#include<stdio.h>
#include<X11/Xlib.h>
#include<X11/IntrinsicP.h>
#include<X11/Shell.h>
#include<StringDefs.h>
#include"Connect.h"
#include"VertexOBJ.h"
#include"VertexDisplay.h"
#include"VertexOper.h"
#include"VertexEdit.h"
#include"VertexFile.h"
#include"VertexMount.h"
#include"VertexRender.h"
#include"VertexColor.h"

/* Function prototype */
void VertexInit(int , int ,char *); 
void VertexColorMap();
void VertexInfo(); 
void VertexScene();
void VertexPalette();
void VertexRenderMat();

#endif
