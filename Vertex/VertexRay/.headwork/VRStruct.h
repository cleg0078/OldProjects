/******************************************/
/*                Vertex 3D               */  
/*            Ray Tracing Module          */
/*               Version 1.0              */
/*----------------------------------------*/
/* Concept And Design:C.LEGENDRE Nov 1995 */
/******************************************/

#ifndef __VRSTRUCT_H
#define __VRSTRUCT_H
#define VDBL double         /* Change To "%f" if You Use VDBL= float */
#define VDBL_FORMAT "%lf"   /* But Don't Now..... There is one BUG */
#define TRUE 1
#define FALSE 0
#ifndef NULL
#define NULL 0
#endif
#define MINT 1e-5
#define NONE    0
#define aX 1
#define aY 2
#define aZ 3
#define MIN(a,b) (a<b?a:b)
#define ABS(x) (x<0.0?-x:x)

#define MAX_OBJ 4000   /* raised 2026-10-21 for the chess scene (was 300) */
#define MAX_LIGHT 20

/* Object Type: Dont'Modify */
#define SPHERE_OBJ     0X0000L
#define BOX_OBJ        0X0001L
#define TORUS_OBJ      0X0002L
#define PLANE_OBJ      0X0003L
#define CYLINDER_OBJ   0X0004L
#define CONE_OBJ       0X0005L
#define TRIANGLE_OBJ   0X0006L


/* Mapping Type */
#define PLANAR 0L
#define SPHERICAL 1L

/* CSG Operation: Don't Modify */
#define UNION      0X0000L
#define INTER      0X0001L

typedef struct image
{ 
 int Type,Interp;
 VDBL Scale_X,Scale_Y; 
 VDBL Rx,Ry;
 unsigned short Width,Height;
 unsigned char *Red,*Green,*Blue;
}VMAPPING;

typedef struct vpt2d
{
 VDBL X,Y;
}VPT2D;

typedef struct vpt3d
{
 VDBL X,Y,Z;
}VPT3D;

typedef struct vector
{
 VDBL X,Y,Z;
}VECTOR;


typedef struct color
{
 VDBL R,G,B;
}COLOR;

typedef struct light
{
 VECTOR org;
 VDBL angle;
 VPT3D Pos,Point_At;
 VDBL Intensity; /* 0.0 ... 100.0 */
 COLOR Color;
}LIGHT;

typedef struct sphere
{
 VECTOR Center;
 VDBL Radius,Radius2;
}SPHERE;
 

typedef struct vquad
{
 VPT3D p1,p2,p3;
}QUAD;


typedef struct triangle
{
 VECTOR p1,p2,p3;
}TRIANGLE;
 
typedef struct obj
{
 int Type;           
 union data {
             SPHERE sphere;
             QUAD quad;
             TRIANGLE triangle;
             } data;

VDBL  reflection;     
VDBL  refraction;     
COLOR refl_color;     
COLOR refr_color;     
COLOR ambient;        
COLOR diffuse;        
COLOR specular;       
VDBL  width;          
VDBL  obindex;          
VDBL  refl_diffuse;   
VDBL  refr_diffuse;   
VECTOR time; 
char Mapping;  /* Object Has Mapping Texture */
VMAPPING map;  /* Mapping Structure */
}OBJ;

typedef struct ray
{
 VECTOR pos;     /* origin */
 VECTOR dir;     /* direction */
 OBJ *obj;       /* where the ray comes from */
 VDBL theta;     /* the diffusion angle */
}RAY;

/* the intersection structure */
typedef struct intersect
{
 OBJ *obj;      /* which object */
 COLOR CMapping; /* Mapping Color */
 VDBL t;        /* where in the ray */
 VECTOR n;      /* the normal at that point */
}INTERSECT;


typedef struct scene
{
 VDBL Minx,Maxx,Miny,Maxy,Minz,Maxz;
 VECTOR Eye,Up,Right;
 VDBL Fov;
}SCENE;

#endif
