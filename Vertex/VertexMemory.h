/*********************************************/
/*    Vertex Memory Allocation Header File   */
/*-------------------------------------------*/
/*              NO COPYRIGHT 1995            */
/*-------------------------------------------*/
/*          C.LEGENDRE && S.MAUFROY          */
/*********************************************/
#ifndef __VERTEX_MEMORY_H
#define __VERTEX_MEMORY_H
#include"VertexStruct.h"
#include"VertexGUI.h"

/* Objects Memory Functions */
int AllocOBJ3D(OBJ3D *,int,int);
void FreeOBJ3D(OBJ3D *);

/* List of OBJ3D */ 
TREE_OBJ3D *AllocTREE_OBJ3D();
void FreeTREE_OBJ3D(TREE_OBJ3D *);
OBJ3D *CreateOBJ3D();
OBJ3D *FindNextPlace();
OBJ3D *FindNamedOBJ(const char *,int);
void DestroyOBJ3D(OBJ3D *);
void DeleteOBJ3D(BOOL);
void VertexDelOBJ();
void VertexNEWAll();

/* "Edit Mode" Functions */
int BuildPTList(PTList **,OBJ3D *);
int BuildFCList(FCList **,FACE3D *,int);
int AddPoint(PTList **,float,float,float);
int DelPoint(PTList **,float,float,float);
int FindPoint(PTList *,float,float,float);
int FindPointXY(PTList *,float,float,float);
int FindPointXZ(PTList *,float,float,float);
int FindPointYZ(PTList *,float,float,float);
int AddFace(FCList **,int ,int,int);
int DelFace(FCList **,int,int,int);
int FindFace(FCList *,int ,int ,int);
int NbFace(FCList *);
int NbPoint(PTList *);
int RebuildOBJ(OBJ3D *,PTList *,FCList *);
void DestroyPTList(PTList **);
void DestroyFCList(FCList **);
int ReBuildOBJ(OBJ3D **,PTList *,FCList *);
void DelAllFaceWith(FCList **,int);

/* Wire Render Memory Allocation */
int AllocWireRender(WireRenderInfo *);
int AllocWireInfo(WireInfo *);
void FreeWireRender(WireRenderInfo *);
void FreeWireInfo(WireInfo *);

/* Extra usefull Functions */
void FindMinMax(OBJ3D *);
void PerformGC(OBJ3D *);    /* gravity Center */
OBJ3D *BuildMinMaxOBJ(struct obj3d *);
int CopyOBJ3D(struct obj3d *,struct obj3d *);
PT3D GetNormal(PT3D *);
float DotProduct(PT3D,PT3D);
void FlipNormal(OBJ3D *);
void VertexFlipNormal();
void VertexNormalUnify();
void UnifyObject(OBJ3D *,PT3D);
#endif
