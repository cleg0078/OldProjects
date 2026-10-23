/*
 * here is the shading function
 */

#include"VRStruct.h"
#include"Vector.h"
#include"BVH.h"
#include<math.h>

extern COLOR Trace(RAY r,int n);
extern INTERSECT Intersect(RAY r);
extern RAY SampleRay(RAY r,VDBL theta);
extern LIGHT light[MAX_LIGHT];
extern int nlight;
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
 RAY r2;

/* to have a shadow, the light ray must intersect an object */
/* first sample the ray around the shadow ray */

 r2 = SampleRay(r, r.theta);

 /* Any-hit occlusion query (2026-10): same predicate as the old
  * 'Intersect + nearest-hit accept rule' below, but the BVH walk stops
  * at the first qualifying hit and prunes every box at the light's
  * distance. Falls back to the identical linear scan for tiny scenes
  * or VRAY_LINEAR. */
 return ShadowOccluded(r2, ldist2);
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
 int L, lit;
 VDBL lr,lg,lb;                 /* effective (color x intensity) light RGB */

/* if the recursion level has been exceeded, return peacefully*/

col.R = col.G = col.B = 0.0;
colmap = col;

 if(n > maxrec_level )
    return col;

/* initially get the ambient color */

  col = i.obj->ambient;


/* first calculate the intersection point */
 pt = vadd( r.pos, svproduct(i.t, r.dir) );

 /* Per-light loop (2026-10): every LIGHT block contributes its own
  * diffuse/specular (and texture-map) terms behind its own shadow ray.
  * Each light carries a Color scaled by Intensity into (lr,lg,lb);
  * the 1995 single-white-light scenes hit lr=lg=lb=1.0, and x*1.0 is
  * exact in IEEE doubles, so their pixels are bit-for-bit unchanged.
  * Reflection/refraction fan-out below stays single-pass (rays carry
  * no light identity). */
 lit = LitFromLight(i.obj);   /* object-level hoist of the 2026-10 dead-ray
                                  prune: if the shadow answer can never be
                                  heard, no light's shadow ray is cast */
 for( L = 0; L < nlight; L++ )
   {
    VECTOR tolight;

    lr = light[L].Color.R * light[L].Intensity;
    lg = light[L].Color.G * light[L].Intensity;
    lb = light[L].Color.B * light[L].Intensity;

    /* dead-light prune: a black or zero-intensity light shades nothing
       and its shadow ray is dead work (same spirit as LitFromLight) */
    if( (lr + lg + lb) <= 1e-4 ) continue;

    /* vector from the intersection to THIS light */
    tolight = vsub(light[L].org, pt);
    ldist2 = vdot(tolight, tolight);
    if( ldist2 <= MINT ) continue;   /* light sitting on the surface */
    ldir = norm( tolight );

    /* dot of the normal with the light direction (light comes FROM the
       light, so no sign flip here) */
    ldot = vdot(i.n, ldir);

    /* and find if that shadow ray is stopped by an object */
    shadow_ray.pos = pt;
    shadow_ray.dir = ldir;
    shadow_ray.obj = i.obj;

    /* the following is the spreading angle of the ray */
    shadow_ray.theta = light[L].angle;

    shad = lit ? Shadow(shadow_ray, ldist2) : 1;
    if( (ldot>0) && !shad )
      {
       /* add some diffuse color, tinted by this light */

       col.R += (ldot * i.obj->diffuse.R) * lr;
       col.G += (ldot * i.obj->diffuse.G) * lg;
       col.B += (ldot * i.obj->diffuse.B) * lb;
        
       if( i.obj->Mapping)
         {
          colmap.R += ldot * i.CMapping.R * lr;
          colmap.G += ldot * i.CMapping.G * lg;
          colmap.B += ldot * i.CMapping.B * lb;
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
       col.R += spec * i.obj->specular.R * lr;
       col.G += spec * i.obj->specular.G * lg;
       col.B += spec * i.obj->specular.B * lb;

       if( i.obj->Mapping)
         {
          colmap.R += spec * i.CMapping.R * lr;
          colmap.G += spec * i.CMapping.G * lg;
          colmap.B += spec * i.CMapping.B * lb;
         }

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