/******************************************/
/*                Vertex 3D               */
/*            Ray Tracing Module          */
/*               Version 1.0              */
/*----------------------------------------*/
/* Concept And Design:C.LEGENDRE Nov 1995 */
/******************************************/

#ifndef __VRDISPLAY_H
#define __VRDISPLAY_H
#include"Connect.h"

extern XGUI gui;
extern XShell *sh;

extern void (*AddPixel)(int,int,unsigned long,unsigned long,unsigned long);
extern void WaitKeyBoard();
extern int InitPlot(int,int);
extern void ClosePlot();

#endif



