/*********************************************/
/*             Vertex COLOR Include          */
/*-------------------------------------------*/
/*              NO COPYRIGHT 1995            */
/*-------------------------------------------*/
/*          C.LEGENDRE && S.MAUFROY          */
/*********************************************/

#ifndef __VERTEX_COLOR_H
#define __VERTEX_COLOR_H
#include<stdio.h>
#include<X11/Xlib.h>
#include<X11/IntrinsicP.h>
#include<X11/Shell.h>
#include<StringDefs.h>
#include"Connect.h"
#include"VertexStruct.h"

/* Function prototype */
unsigned long GetRGB24_8(int,int,unsigned char,unsigned char,unsigned char);
unsigned long GetRGB24_16(int,int,unsigned char,unsigned char,unsigned char);
unsigned long GetRGB24_24(int,int,unsigned char,unsigned char,unsigned char);
void InitRGBDisplay(void);   /* defined in VertexColor.c, called at startup */
void VertexColor();

#endif
