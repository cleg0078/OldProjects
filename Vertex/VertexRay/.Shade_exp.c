/*
 * here is the shading function
 */

#include"VRStruct.h"
#include"Vector.h"
#include<math.h>

extern COLOR Trace(RAY r,int n);
extern INTERSECT Intersect(RAY r);
extern RAY SampleRay(RAY r,VDBL theta);
extern LIGHT light[MAX_LIGHT];
extern int maxrec_level;

/*
 * Shadow check.  Check if the ray from the intersection point to the
 * light, intersection any objects inbetween.  If it does, we have a
 * shadow.
 * One improvement that can be made is to check the transparencies of all
 * the intersecting objects and make the shadow have a different intensity
 * depending on the transparency of the objects
 */

int Shadow(RAY r,VDBL ldist2)
{
 INTERSECT i;
 RAY r2;

/* to have a shadow, the light ray must intersect an object */
/* first sample the ray around the shadow ray */

 r2 = SampleRay(r, r.theta);
 i = Intersect(r2);

 if( i.obj == NULL )
    return FALSE;

 /* squared distance to the light: an occluder beyond it must not shade */
 if( i.t*i.t >= ldist2 )
    return FALSE;

 return (r.obj != i.obj);
}

/*
 * LitFromLight: can the shadow test change this surface's color at all?
 * The shadow flag gates exactly the lit block below (diffuse + specular,
 * direct and via their texture-map terms). Reflection/refraction fan-outs
 * are NOT gated by it. Ambient never is. So a surface with zero diffuse
 * and zero specular in every channel cannot see the shadow ray's answer:
 * skip the ray. Epsilon guards denormals; shipped materials are 0 or >=0.01.
 */
static int LitFromLight(OBJ *obj)
{
 return (obj->diffuse.R + obj->diffuse.G + obj->diffuse.B
       + obj->specular.R + obj->specular.G + obj->specular.B) > 1e-4;
}

/*
 * calculate the reflection vector.
 * D. Rogers: "Procedural elements for computer graphics". 5-12. p. 367
 */
VECTOR Reflect(VECTOR n,VECTOR v1)
{
 VECTOR v2;

 v2 = vsub( v1, svproduct( 2 * vdot(n, v1), n) );

 #ifdef __DEBUG
 printf("Reflection :ok\n");
 #endif

 return v2;
}

/*
 * calculate the refracted vector.
 * D. Rogers: "Procedural elements for computer graphics". 5-12. p. 367
 */

VECTOR Refract(VECTOR n,VECTOR v1,VDBL index)
{
 VDBL p, t;
 VECTOR v2;

 v2.X = 0.; v2.Y = 0.; v2.Z = 0.;

 if( index <= 0.0 )
    return v2;   /* no refractive index: skip instead of dividing by zero */

 p = vdot(n, v1);
 if(p < 0)
   {
    t = 1 - ( 1 - p*p ) / ( index*index );
    if( t <= 0 )
       return v2;

    t = -p/index - sqrt(t);
   }
 else
  {
   index = 1 / index;
   t = 1 - ( 1 - p*p ) / ( index*index );
   if( t <= 0 )
      return v2;

   t = -p/index + sqrt(t);
  }

 v2 = vadd( svproduct(1/index, v1), svproduct(t, n) );

 #ifdef __DEBUG
 printf("Refraction :ok\n");
 #endif
 
 return v2;
}

/*
 * the actual shading function.  Recursively calls trace()
 */
COLOR Shade(INTERSECT i,RAY r,int n)
{
 COLOR col,colmap;
 VECTOR	pt;
 RAY shadow_ray;
 VECTOR ldir;
 int shad;
 VDBL ldot,max,ldist2;
 VDBL spec;
 VECTOR rr;                     /* reflected light direction */
 RAY reflected, refracted;      /* refl, refr directions */
 COLOR c;

/* if the recursion level has been exceeded, return peacefully*/

col.R = col.G = col.B = 0.0;
colmap = col;

 if(n > maxrec_level )
    return col;

/* initially get the ambient color */

  col = i.obj->ambient;


/* first calculate the intersection point */
 pt = vadd( r.pos, svproduct(i.t, r.dir) );

/* for the light source first get the vector from it to the intersection */
 {
  VECTOR tolight = vsub(light[0].org, pt);
  ldist2 = vdot(tolight, tolight);
  ldir = norm( tolight );
 }

/* then calc the dot product between the light direction and the normal   */
/* negative because the light direction is reverse (comes from the light) */
 ldot = vdot(i.n, ldir);

/* and find if that shadow ray is stopped by an object */
 shadow_ray.pos = pt;
 shadow_ray.dir = ldir;
 shadow_ray.obj = i.obj;

/* the following is the spreading angle of the ray */
 shadow_ray.theta = light[0].angle;

 /* Dead-ray prune (2026-10): the shadow result only ever scales the
  * diffuse/specular (and their map) contributions and the two diffuse
  * reflection/refraction channels below. If every one of those weights
  * is zero, the surface reflects light nowhere it is heard: skip the
  * shadow ray. (Ambient-only surfaces — the starfield's trick — land
  * exactly here.) */
 shad = LitFromLight(i.obj) ? Shadow(shadow_ray, ldist2) : 1;
 if( (ldot>0) && !shad )
   {
    /* add some diffuse color. */

    col.R += (ldot * i.obj->diffuse.R);
    col.G += (ldot * i.obj->diffuse.G);
    col.B += (ldot * i.obj->diffuse.B);
     
    if( i.obj->Mapping)
      {
       colmap.R += ldot * i.CMapping.R;
       colmap.G += ldot * i.CMapping.G;
       colmap.B += ldot * i.CMapping.B;
      }

    /* now calc the specular color */
    /* first calculate the reflected light vector */
    rr = Reflect(i.n, ldir);

    /* then take the dot of the reflected ray with the viewing direction */
    /* that is the specular component. The minus is there for obvious reasons */
    spec = vdot(rr, r.dir);
    spec = (spec < 0) ? 0 : spec;
           
    /* remember the specular width factor?  We use it here! */
    spec = pow(spec, i.obj->width);
    col.R += spec * i.obj->specular.R;
    col.G += spec * i.obj->specular.G;
    col.B += spec * i.obj->specular.B;

    if( i.obj->Mapping)
      {
       colmap.R += spec * i.CMapping.R;
       colmap.G += spec * i.CMapping.G;
       colmap.B += spec * i.CMapping.B;
      }

   }

 /* setup the reflected ray — only when reflection has any weight
 (dead-ray prune 2026-10: the 1995 REFLECTION coefficient exists to
 weight this term; a zero weight makes the whole ray dead work) */
 if( i.obj->reflection >= 1e-4 )
   {
  reflected.pos = pt;
  reflected.dir = Reflect(i.n, r.dir);
  reflected.obj = i.obj;
  reflected.theta = i.obj->refl_diffuse;
 
  /* calculate the reflection */
  c = Trace(reflected, n+1);

  col.R += c.R * i.obj->refl_color.R * i.obj->reflection;
  col.G += c.G * i.obj->refl_color.G * i.obj->reflection;
  col.B += c.B * i.obj->refl_color.B * i.obj->reflection;

  if( i.obj->Mapping)
    {
     colmap.R += c.R * i.CMapping.R *i.obj->reflection ;
     colmap.G += c.G * i.CMapping.G *i.obj->reflection;
     colmap.B += c.B * i.CMapping.B *i.obj->reflection;
    }
   }

 /* now setup the refracted ray — only when refraction has any weight
 (dead-ray prune 2026-10, same rationale as the reflection prune) */
 if( i.obj->refraction >= 1e-4 )
   {
 refracted.pos = pt;
 refracted.dir = Refract(i.n, r.dir, i.obj->obindex);
 refracted.obj = i.obj;
 refracted.theta = i.obj->refr_diffuse;

 if( (refracted.dir.X == 0.) && (refracted.dir.Y == 0.) && (refracted.dir.Z == 0.) )
    return col;   /* matches original: un-mapped col, as pre-prune code did */
 
 /* and calculate the refraction */
 c = Trace(refracted, n+1);

    col.R += c.R * i.obj->refr_color.R * i.obj->refraction;
    col.G += c.G * i.obj->refr_color.G * i.obj->refraction;
    col.B += c.B * i.obj->refr_color.B * i.obj->refraction;

 if( i.obj->Mapping)
   {
    colmap.R += c.R * i.CMapping.R *i.obj->refraction;
    colmap.G += c.G * i.CMapping.G *i.obj->refraction;
    colmap.B += c.B * i.CMapping.B *i.obj->refraction;
   }
   }

 if( i.obj->Mapping)
   {
    col.R += colmap.R;
    col.G += colmap.G;
    col.B += colmap.B;
   }

 return col;
}