/*
 * triangle() primitive unit harness - not part of libVRay (RAYOBJ).
 * Links against the library for Intersect(); no X calls are made.
 *
 * build: gcc -O2 triangletest.c -o triangletest -I. -I../XGUI \
 *            -L. -L../XGUI -lVRay -lXGUI -lX11 -lm
 */

#include"VRay.h"
#include<math.h>
#include<stdio.h>

/* no header declares it today - same extern Shade.c uses */
extern INTERSECT Intersect(RAY r);

static int fails = 0;

static void expect_hit(const char *name,RAY r,VDBL t_ref,VDBL nx,VDBL ny,VDBL nz)
{
 INTERSECT i = Intersect(r);

 if( i.obj == NULL )
    { printf("FAIL %s: expected hit, got miss\n",name); fails++; return; }
 if( fabs(i.t-t_ref) > 1e-9 )
    { printf("FAIL %s: t=%.12f expected %.12f\n",name,i.t,t_ref); fails++; return; }
 if( fabs(i.n.X-nx)>1e-9 || fabs(i.n.Y-ny)>1e-9 || fabs(i.n.Z-nz)>1e-9 )
    { printf("FAIL %s: normal (%f,%f,%f) expected (%f,%f,%f)\n",name,
             i.n.X,i.n.Y,i.n.Z,nx,ny,nz); fails++; return; }
 printf("PASS %s\n",name);
}

static void expect_miss(const char *name,RAY r)
{
 INTERSECT i = Intersect(r);

 if( i.obj != NULL )
    { printf("FAIL %s: expected miss, got hit t=%f\n",name,i.t); fails++; return; }
 printf("PASS %s\n",name);
}

static void set_tri(VDBL x2,VDBL y2,VDBL z2,VDBL x3,VDBL y3,VDBL z3)
{
 obj[0].Type = TRIANGLE_OBJ;
 obj[0].Mapping = 0;
 obj[0].data.triangle.p1.X = 0.0; obj[0].data.triangle.p1.Y = 0.0; obj[0].data.triangle.p1.Z = 0.0;
 obj[0].data.triangle.p2.X = 1.0; obj[0].data.triangle.p2.Y = 0.0; obj[0].data.triangle.p2.Z = 0.0;
 obj[0].data.triangle.p3.X = x2;  obj[0].data.triangle.p3.Y = y2;  obj[0].data.triangle.p3.Z = z2;
 /* p3 unused above: p2 is (1,0,0); p3 set below */
 obj[0].data.triangle.p3.X = x3;  obj[0].data.triangle.p3.Y = y3;  obj[0].data.triangle.p3.Z = z3;
 nobj = 1;
}

int main(void)
{
 RAY r;

 r.obj = NULL;
 r.theta = 0;

 /* triangle p1(0,0,0) p2(1,0,0) p3(0,1,0) in the z=0 plane */
 set_tri(1.0,0.0,0.0, 0.0,1.0,0.0);

 /* straight-on hit inside the triangle. The two-sided rule (quad-compatible)
    keeps dot(n,dir)<=0: the normal always faces the side the ray came from, */
 r.pos.X=0.25; r.pos.Y=0.25; r.pos.Z=1.0;
 r.dir.X=0.0;  r.dir.Y=0.0;  r.dir.Z=-1.0;
 expect_hit("hit_front_normal_faces_ray_origin",r,1.0, 0.0,0.0,1.0);

 /* outside hypotenuse: u+v>1 */
 r.pos.X=0.6; r.pos.Y=0.6; r.pos.Z=1.0;
 expect_miss("miss_outside_hypotenuse",r);

 /* back side: two-sided, normal faces the ray origin (z<0) => -z */
 r.pos.X=0.25; r.pos.Y=0.25; r.pos.Z=-1.0;
 r.dir.X=0.0;  r.dir.Y=0.0;  r.dir.Z=1.0;
 expect_hit("hit_back_normal_z-1",r,1.0, 0.0,0.0,-1.0);

 /* origin inside the plane, ray +z: plane hit at t=0 must miss (behind/leaving) */
 r.pos.X=0.25; r.pos.Y=0.25; r.pos.Z=0.0;
 expect_miss("miss_in_plane_t0",r);

 /* ray parallel to the plane: det guard */
 r.pos.X=0.25; r.pos.Y=0.25; r.pos.Z=1.0;
 r.dir.X=1.0;  r.dir.Y=0.0;  r.dir.Z=0.0;
 expect_miss("miss_parallel_ray",r);

 /* degenerate: collinear p3(2,0,0) - zero area */
 set_tri(1.0,0.0,0.0, 2.0,0.0,0.0);
 r.pos.X=0.25; r.pos.Y=0.25; r.pos.Z=1.0;
 r.dir.X=0.0;  r.dir.Y=0.0;  r.dir.Z=-1.0;
 expect_miss("miss_degenerate_collinear",r);

 /* degenerate: two coincident vertices p3==p1 */
 set_tri(1.0,0.0,0.0, 0.0,0.0,0.0);
 expect_miss("miss_degenerate_coincident",r);

 /* hit exactly on an edge vertex is legal (u=0,v=0 corner, straight at p1
    but t guard: ray starts above p1 pointing down, u=v=0, inside) */
 set_tri(1.0,0.0,0.0, 0.0,1.0,0.0);
 r.pos.X=0.000001; r.pos.Y=0.000001; r.pos.Z=2.0;
 r.dir.X=0.0;      r.dir.Y=0.0;      r.dir.Z=-1.0;
 expect_hit("hit_near_corner",r,2.0, 0.0,0.0,1.0);

 if( fails )
    { printf("%d FAILURE(S)\n",fails); return 1; }
 printf("ALL PASS\n");
 return 0;
}
