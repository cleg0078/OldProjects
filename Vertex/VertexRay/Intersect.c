/*
 * Intersection routines
 */

#include"VRStruct.h"
#include"Vector.h"
#include"BVH.h"
#include<math.h>
#include<stdio.h>
#include<stdlib.h>

extern int nobj;
extern OBJ obj[MAX_OBJ];
extern int SphericalMap(OBJ *,VDBL,VDBL,VDBL,INTERSECT *);
extern int PlanarMap(OBJ *,VDBL,VDBL,VDBL,INTERSECT *);
 
/* 
 * Intersect ray with sphere
 */

INTERSECT sphere(OBJ *obj,RAY r)
{
 VECTOR v;
 VECTOR n; /* normal vector */
 INTERSECT i;
 VDBL b, c, d;
 VDBL sol1, sol2;
 VECTOR center;

 i.obj = NULL;

/* find out what the center is at this time */
 center = obj->data.sphere.Center;

 v = vsub( r.pos, center );
 b = 2 * vdot( r.dir, v );
 c = vdot(v, v) - obj->data.sphere.Radius * obj->data.sphere.Radius;
 /* the c term needs Radius^2; using Radius rendered every sphere at its
    square root (fixed 2026 review; the .vr scenes were retuned to match) */

 d = b * b - 4 * c;
 if( d < 0 )
    return i;
 
 d = sqrt(d);
 sol1 = ( -b + d ) / 2;
 sol2 = ( -b - d ) / 2;
 if( sol1 <= 0 )
    sol1 = sol2;
 if( sol2 <= 0 )
    sol2 = sol1;
 
 i.t = (sol1 < sol2) ? sol1 : sol2 ;

/* if intersection is behind eye */
 if(i.t <= 0)
    return i;
 
 i.obj = obj;	

 /* calculate the normal.  It is just the direction of the radius */
 n = vsub(vadd(r.pos, svproduct(i.t, r.dir)), center);
 i.n = norm(n);

 if( obj->Mapping )
     SphericalMap(obj,n.X,n.Y,n.Z,&i); 
 
 return i;
}

/*
 * intersect ray with a quadrangle
 */

INTERSECT quad(OBJ *obj,RAY r)
{
 INTERSECT i;
 VDBL x0, y0, z0;
 VDBL dx1, dy1, dz1, dx2, dy2, dz2, qx, qy, qz;
 VDBL alpha, a, b, t;
 VDBL d, dalpha, da, db;
 VECTOR n;
 VDBL pdx, pdy, pdz;
 VDBL size,tmp1,tmp2,tmp3,tmp4,tmp5;

 i.obj = NULL;

 x0 = obj->data.quad.p1.X;
 y0 = obj->data.quad.p1.Y;
 z0 = obj->data.quad.p1.Z;
 dx1 = x0 - obj->data.quad.p2.X;
 dy1 = y0 - obj->data.quad.p2.Y;
 dz1 = z0 - obj->data.quad.p2.Z;
 dx2 = x0 - obj->data.quad.p3.X;
 dy2 = y0 - obj->data.quad.p3.Y;
 dz2 = z0 - obj->data.quad.p3.Z;
 
 qx = r.dir.X;
 qy = r.dir.Y;
 qz = r.dir.Z;

 pdx = x0 - r.pos.X;
 pdy = y0 - r.pos.Y;
 pdz = z0 - r.pos.Z;

 tmp1 = ( dy1 * dz2 ) - ( dy2 * dz1 );
 tmp2 = ( qy * dz2 ) - ( qz * dy2 );  
 tmp3 = ( qy * dz1 ) - ( qz * dy1 );

 d = qx * tmp1 - dx1 * tmp2 + dx2 * tmp3;

/* if no intersection */
 if( ABS(d) < MINT )
    return i;

 tmp4 =  ( pdy * dz2 ) - ( pdz * dy2 );
 dalpha = pdx * tmp1 -dx1 * tmp4 + dx2 * ( ( pdy * dz1 ) - ( pdz * dy1 ) ) ;

 alpha = dalpha / d;

/* if intersection behind the eye */
 if( alpha <= 0 )
    return i;

 tmp5 = ( qy * pdz ) - ( qz * pdy );

 da = qx * tmp4 - pdx * tmp2 + dx2 * tmp5; 
 db = qx * ( ( dy1 * pdz ) - ( dz1 * pdy ) ) - dx1 * tmp5 + pdx * tmp3;

 a = da / d;
 b = db / d;

/* check if intersection within plane */
 if( ( a < 0 ) || ( a > 1 ) )
    return i;

 if( ( b < 0 ) || ( b > 1 ) )
    return i;

 /* assign the object */
 i.obj = obj;
 i.t = alpha;

 /* normal to the plane */
 n.X = dy1 * dz2 - dy2 * dz1;
 n.Y = - ( dx1 * dz2 - dx2 * dz1 );
 n.Z = dx1 * dy2 - dx2 * dy1;
 i.n = norm(n);

 /* reverse the normal if desired */
 if( vdot( i.n, r.dir ) > 0 )
    i.n = vneg( i.n );


 if( obj->Mapping )
    PlanarMap(obj,r.dir.X,r.dir.Y,r.dir.Z,&i);

 return i;
}


/*
 * intersect ray with a triangle
 * Moller-Trumbore, same parameterization as quad() above: t along the
 * ray, u/v barycentric on the two edges leaving p1. Zero-area triangles
 * and coplanar rays miss cleanly (det guard / t guard).
 */

INTERSECT triangle(OBJ *obj,RAY r)
{
 INTERSECT i;
 VECTOR p1,p2,p3,e1,e2,s,pv,qv,n;
 VDBL det,a,u,v,t;

 i.obj = NULL;

 p1 = obj->data.triangle.p1;
 p2 = obj->data.triangle.p2;
 p3 = obj->data.triangle.p3;

 e1 = vsub(p2, p1);
 e2 = vsub(p3, p1);

 pv = vcross(r.dir, e2);
 det = vdot(e1, pv);

 /* parallel ray, or degenerate (zero-area) triangle */
 if( ABS(det) < MINT )
    return i;

 a = 1.0 / det;

 s = vsub(r.pos, p1);
 u = a * vdot(s, pv);

 /* outside the p1->p2 edge */
 if( (u < 0.0) || (u > 1.0) )
    return i;

 qv = vcross(s, e1);
 v = a * vdot(r.dir, qv);

 /* outside the p2->p3 edge */
 if( (v < 0.0) || (u + v > 1.0) )
    return i;

 t = a * vdot(e2, qv);

 /* if intersection behind the eye */
 if( t <= 0 )
    return i;

 i.obj = obj;
 i.t = t;

 /* normal to the plane */
 n = vcross(e1, e2);
 i.n = norm(n);

 /* reverse the normal if desired (two-sided, as quad() does) */
 if( vdot( i.n, r.dir ) > 0 )
    i.n = vneg( i.n );

 if( obj->Mapping )
    PlanarMap(obj,r.dir.X,r.dir.Y,r.dir.Z,&i);

 return i;
}


static INTERSECT LinearIntersect(RAY r)
{
int i;
INTERSECT inter, intermin;

intermin.obj = NULL;

 for(i = 0; i < nobj; i++)
    {
     /* choose the appropriate routine for each object */
     switch( obj[i].Type )
           {
            case(SPHERE_OBJ):
                 inter = sphere( &obj[i], r);
                 break;
            case(PLANE_OBJ):
                 inter = quad( &obj[i], r);
                 break;
            case(TRIANGLE_OBJ):
                 inter = triangle( &obj[i], r);
                 break;
            default:
                 inter.obj = NULL;
	   }
 
    /* update the minimum intersection distance */
    if( inter.obj && (!intermin.obj || (inter.t < intermin.t)) && (inter.obj!=r.obj))
       intermin = inter;
   }
 return intermin;
}

/*
 * Public entry: BVH traversal when a tree covers the current scene,
 * plain scan otherwise (tiny scenes, hand-built obj[] previews, or a
 * scene too large for the static tree arrays). BVHValid() lazily
 * rebuilds when nobj moved since the last BuildBVH().
 */
INTERSECT Intersect(RAY r)
{
 static int no_bvh = -1;
 if( no_bvh == -1 )
    no_bvh = ( getenv("VRAY_LINEAR") != NULL );
 if( !no_bvh && nobj > BVH_MIN_OBJ && BVHValid() )
    return BVHIntersect(r);
 return LinearIntersect(r);
}
