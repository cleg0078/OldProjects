/*********************************************/
/*          Globals Vertex Structures        */
/*-------------------------------------------*/
/*              NO COPYRIGHT 1995            */
/*-------------------------------------------*/
/*          C.LEGENDRE && S.MAUFROY          */
/*********************************************/
#ifndef __VERTEX_STRUCT_H
#define __VERTEX_STRUCT_H
#include<malloc.h>
#include<stdlib.h>
#include<stdio.h>
#include<math.h>

/* Some usefull DEFINE */
#define BOOL char 
#define TRUE 1
#define FALSE 0
#define MagicFile (int)'V'<<8|'T'

/* 3D Point*/
typedef struct pt3d
{
 float X,Y,Z;
}PT3D;


/* 3D face */
typedef struct face3d
{
 int I1,I2,I3;
}FACE3D;


/* RGB Color definition */
typedef struct rgbcolor
{
 unsigned char Red,Green,Blue;
}RGBColor;


/* 3D Object */
typedef struct obj3d
{
 char Type;               /* Disk,Sphere,Cube,Personal... */
 PT3D Center;             /* Spacial postion (Gravity Center)*/
 PT3D Min,Max;            /* Global Volume */
 char Name[25];           /* Object Name */
 int NBPT3D;              /* Number of PT3D  */
 int NBFACE;              /* number of faces */
 struct pt3d *PTtable;    /* pointer on the 3D object points */
 struct face3d *FCtable;  /* pointer on the 3D object faces  */
 struct obj3d *MMObj1;    /* Original Min-Max Object */
 struct obj3d *MMObj2;    /* Real Time Min-Max Object*/
 char Unified;            /* Normal Unify */
 PT3D *Normal;            /* Face Normal */
 PT3D *VertexNormal;      /* VertexNormal */
 RGBColor *Color;         /* Color Of The Face Of The Object 24 bits */
}OBJ3D;
 
/* 3D Object List */
typedef struct tree_obj3d
{
 struct tree_obj3d *Brother;
 struct tree_obj3d *Soon;
 OBJ3D *Actual;
}TREE_OBJ3D;

/* 3D Scene */
typedef struct scene3d
{
 TREE_OBJ3D *Head; /* Beginning of The 3D Scene */ 
 TREE_OBJ3D *Tail; /* For Speed */
 PT3D Observer;    /* observer position */
 PT3D LookAt;      /* observer taget */
 PT3D Univers_Max,Univers_Min; /* World Dimension */
}SCENE3D;

/* "Edit Mode" Structure */
typedef struct ptlist
{
 struct ptlist *Next;
 PT3D Info;
}PTList;

typedef struct fclist
{
 struct fclist *Next;
 FACE3D Info;
}FCList;

typedef struct FunctionList
{
 void *fct;
}FCTLIST;

typedef struct winfo
{
 float Distance;
 int ind;
}WireInfo;

typedef struct wrinfo
{
 struct wrinfo *Next;          /* Pointer On The Next REnder Object */
 WireInfo *Winf;               /* Wire Object Info                  */
 OBJ3D *Obj;                   /* render Object                     */  
 int start,end;                /* First And End Face                */
}WireRenderInfo; 

#endif 
