/*
 * Bounding-volume hierarchy (BVH) over the global object list.
 *
 * Replaces the per-ray O(n) scan for scenes over BVH_MIN_OBJ objects.
 * Built once per parsed scene (Parse calls BuildBVH at end of parse);
 * Intersect() re-validates lazily through BVHValid() so callers that
 * fill obj[] by hand (VertexMat previews, triangletest) keep working.
 *
 * Tree: median split on the longest centroid axis, leaf size 4.
 * Traversal: iterative, slab test per node box, near-child first.
 * Hit semantics are identical to the linear loop, including the
 * "inter.obj != r.obj" exclusion and the strict-less-than rule that
 * makes first-in-obj[]-order win exact-t ties (coplanar floor quads
 * and piece bases sit at the same y=0 in chess.vr).
 */

#include"VRStruct.h"
#include"Vector.h"
#include"BVH.h"
#include<stdlib.h>
#include<float.h>
#include<math.h>

extern int nobj;
extern OBJ obj[MAX_OBJ];

extern INTERSECT sphere(OBJ *obj,RAY r);
extern INTERSECT quad(OBJ *obj,RAY r);
extern INTERSECT triangle(OBJ *obj,RAY r);

#define BV_LEAF_SIZE 4
#define BV_MAX_DEPTH 40
#define BV_NODE_MAX (2*MAX_OBJ)   /* L leaves + L-1 internal <= 2n-1 */
#define BV_PAD 1e-3        /* box inflation: cull-only, never adds hits */
#define BV_FAR_T 1e300

typedef struct { VDBL mn[3], mx[3]; } BVBOX;

typedef struct
{
 BVBOX box;
 int left, right;         /* child node indices; left<0 marks a leaf */
 int start, count;        /* leaf: range into bv_perm */
} BVNode;

static BVNode bv_node[BV_NODE_MAX];
static int    bv_perm[MAX_OBJ];
static BVBOX  bv_box[MAX_OBJ];
static int    bv_node_count;
static int    bv_root;
static int    bv_built_nobj = -1;     /* -1: no usable tree */
static int    bv_axis;                /* axis for the active qsort */

/* raw coords: quad/triangle data members are VPT3D/VECTOR — distinct types */
static void ExtendPoint(BVBOX *b, VDBL x, VDBL y, VDBL z)
{
 if( x < b->mn[0] ) b->mn[0] = x;
 if( x > b->mx[0] ) b->mx[0] = x;
 if( y < b->mn[1] ) b->mn[1] = y;
 if( y > b->mx[1] ) b->mx[1] = y;
 if( z < b->mn[2] ) b->mn[2] = z;
 if( z > b->mx[2] ) b->mx[2] = z;
}

/* box of one object, padded. Unknown types come out empty (never hit). */
static void BoxOf(int i, BVBOX *b)
{
 int a;
 for( a=0; a<3; a++ ) { b->mn[a]=DBL_MAX; b->mx[a]=-DBL_MAX; }

 switch( obj[i].Type )
 {
  case(SPHERE_OBJ):
   {
    VECTOR c = obj[i].data.sphere.Center;
    VDBL rad = obj[i].data.sphere.Radius;
    if( rad < 0 ) rad = -rad;
    ExtendPoint(b, c.X, c.Y, c.Z);
    for( a=0; a<3; a++ ) { b->mn[a]-=rad; b->mx[a]+=rad; }
    break;
   }
  case(PLANE_OBJ):
   {
    VPT3D p1 = obj[i].data.quad.p1;
    VPT3D p2 = obj[i].data.quad.p2;
    VPT3D p3 = obj[i].data.quad.p3;
    /* quad() is a parallelogram p1,p2,p3 + implicit p2+p3-p1: box all 4 */
    ExtendPoint(b, p1.X, p1.Y, p1.Z);
    ExtendPoint(b, p2.X, p2.Y, p2.Z);
    ExtendPoint(b, p3.X, p3.Y, p3.Z);
    ExtendPoint(b, p2.X+p3.X-p1.X, p2.Y+p3.Y-p1.Y, p2.Z+p3.Z-p1.Z);
    break;
   }
  case(TRIANGLE_OBJ):
   {
    {
     VECTOR t1 = obj[i].data.triangle.p1;
     VECTOR t2 = obj[i].data.triangle.p2;
     VECTOR t3 = obj[i].data.triangle.p3;
     ExtendPoint(b, t1.X, t1.Y, t1.Z);
     ExtendPoint(b, t2.X, t2.Y, t2.Z);
     ExtendPoint(b, t3.X, t3.Y, t3.Z);
    }
    break;
   }
  default:
   return;   /* empty box (mn>mx): culled by the RayBox guard, never poisons parent unions */
 }

 for( a=0; a<3; a++ ) { b->mn[a]-=BV_PAD; b->mx[a]+=BV_PAD; }
}

static VDBL BoxCenterAxis(int idx, int a)
{
 return 0.5 * ( bv_box[idx].mn[a] + bv_box[idx].mx[a] );
}

static int bv_cmp(const void *p, const void *q)
{
 int ia = *(const int*)p, ib = *(const int*)q;
 VDBL ca = BoxCenterAxis(ia, bv_axis), cb = BoxCenterAxis(ib, bv_axis);
 if( ca < cb ) return -1;
 if( ca > cb ) return 1;
 return ia - ib;   /* deterministic order for coincident centroids */
}

static int BuildNode(int s, int e, int depth)
{
 int self = bv_node_count++;
 int i, a, mid, axis;
 BVBOX *n = &bv_node[self].box;
 BVBOX cb;
 VDBL best, ext;

 for( a=0; a<3; a++ )
   {
    n->mn[a]=DBL_MAX; n->mx[a]=-DBL_MAX;
    cb.mn[a]=DBL_MAX; cb.mx[a]=-DBL_MAX;
   }

 for( i=s; i<e; i++ )
   {
    BVBOX *b = &bv_box[bv_perm[i]];
    for( a=0; a<3; a++ )
      {
       if( b->mn[a] < n->mn[a] ) n->mn[a] = b->mn[a];
       if( b->mx[a] > n->mx[a] ) n->mx[a] = b->mx[a];
      }
   }

 if( (e-s) <= BV_LEAF_SIZE || depth >= BV_MAX_DEPTH )
   {
    bv_node[self].left  = -1;
    bv_node[self].right = -1;
    bv_node[self].start = s;
    bv_node[self].count = e-s;
    return self;
   }

 /* centroid bounds -> split on the longest axis (median, not SAH:
    scene sizes here make SAH irrelevant and keep the build deterministic) */
 for( i=s; i<e; i++ )
   {
    for( a=0; a<3; a++ )
      {
       VDBL c = BoxCenterAxis(bv_perm[i], a);
       if( c < cb.mn[a] ) cb.mn[a] = c;
       if( c > cb.mx[a] ) cb.mx[a] = c;
      }
   }
 axis = 0; best = -1.0;
 for( a=0; a<3; a++ )
   {
    ext = cb.mx[a] - cb.mn[a];
    if( ext > best ) { best = ext; axis = a; }
   }

 bv_axis = axis;
 qsort( &bv_perm[s], (size_t)(e-s), sizeof(int), bv_cmp );
 mid = (s+e)/2;   /* strictly inside (s,e): only reached with e-s > BV_LEAF_SIZE */

 bv_node[self].start = -1;
 bv_node[self].count = 0;
 bv_node[self].left  = BuildNode(s, mid, depth+1);
 bv_node[self].right = BuildNode(mid, e, depth+1);
 return self;
}

static int bv_stale = 0;   /* set by BVHInvalidate(); cleared by BuildBVH */

/*
 * Rebuild the tree for the current obj[0..nobj). Safe to call any time:
 * small or oversized scenes simply get no tree (BVHValid stays false).
 */
void BuildBVH(void)
{
 int i;
 bv_stale = 0;
 bv_built_nobj = -1;
 if( nobj <= BVH_MIN_OBJ || nobj > MAX_OBJ )
    return;

 for( i=0; i<nobj; i++ )
   {
    BoxOf(i, &bv_box[i]);
    bv_perm[i] = i;
   }

 bv_node_count = 0;
 bv_root = BuildNode(0, nobj, 0);
 if( bv_node_count > BV_NODE_MAX )
    return;                       /* paranoid: keep the linear path */

 bv_built_nobj = nobj;
}

/* 1 if a usable tree matches the current nobj (rebuilds when stale).
 * Staleness is keyed on nobj only: a caller that edits obj[] geometry
 * without changing nobj must call BVHInvalidate() first. */
void BVHInvalidate(void)
{
 bv_stale = 1;
}

int BVHValid(void)
{
 if( bv_stale ) { BuildBVH(); bv_stale = 0; }
 if( bv_built_nobj == nobj )
    return 1;
 BuildBVH();
 return bv_built_nobj == nobj;
}

/*
 * Slab test. On success *tent holds the box entry distance (>=0);
 * caller seeds it with the best t found so far, so boxes entered
 * at or beyond that distance prune the subtree.
 * Axis-parallel components take the branch (no inf/NaN tricks).
 */
static int RayBox(BVBOX *b, RAY r, VDBL *tent)
{
 VDBL tn = 0.0, tf = *tent, t0, t1, tmp, inv, o, d;
 int a;

 /* inverted empty box (unknown obj type) would PASS the slab swap for d!=0 */
 if( b->mn[0] > b->mx[0] )
    return 0;

 for( a=0; a<3; a++ )
   {
    o = (a==0)? r.pos.X : ((a==1)? r.pos.Y : r.pos.Z);
    d = (a==0)? r.dir.X : ((a==1)? r.dir.Y : r.dir.Z);
    if( d != 0.0 )
      {
       inv = 1.0/d;
       t0 = (b->mn[a]-o)*inv;
       t1 = (b->mx[a]-o)*inv;
       if( t0 > t1 ) { tmp = t0; t0 = t1; t1 = tmp; }
       if( t0 > tn ) tn = t0;
       if( t1 < tf ) tf = t1;
       if( tn > tf )
          return 0;
      }
    else if( o < b->mn[a] || o > b->mx[a] )
       return 0;
   }
 *tent = tn;
 return 1;
}

/* projected distance from the ray origin to a node box center: near-first order */
static VDBL BoxProj(int node, RAY r)
{
 BVBOX *b = &bv_node[node].box;
 VECTOR c;
 c.X = 0.5*(b->mn[0]+b->mx[0]);
 c.Y = 0.5*(b->mn[1]+b->mx[1]);
 c.Z = 0.5*(b->mn[2]+b->mx[2]);
 return vdot( vsub(c, r.pos), r.dir );
}

INTERSECT BVHIntersect(RAY r)
{
 INTERSECT best, hit;
 int stack[64];   /* tree depth is capped at BV_MAX_DEPTH=40: 64 slots always free */
 int sp = 0;

 best.obj = NULL;
 stack[sp++] = bv_root;

 while( sp > 0 )
   {
    BVNode *nd = &bv_node[stack[--sp]];
    VDBL tent = best.obj ? best.t : BV_FAR_T;

    if( ! RayBox(&nd->box, r, &tent) )
       continue;

    if( nd->left < 0 )
      {
       int i, e = nd->start + nd->count;
       for( i=nd->start; i<e; i++ )
         {
          OBJ *o = &obj[ bv_perm[i] ];
          switch( o->Type )
                {
                 case(SPHERE_OBJ):
                      hit = sphere(o, r);
                      break;
                 case(PLANE_OBJ):
                      hit = quad(o, r);
                      break;
                 case(TRIANGLE_OBJ):
                      hit = triangle(o, r);
                      break;
                 default:
                      hit.obj = NULL;
                }
          if( hit.obj && (hit.obj != r.obj) )
             if( !best.obj || (hit.t < best.t) ||
                 ( (hit.t == best.t) && (hit.obj < best.obj) ) )
                    /* obj[] is contiguous: pointer order == array order,
                       so this reproduces the scan's first-wins exact-t rule */
                best = hit;
         }
      }
    else
      {
       int l = nd->left, rt = nd->right;
       if( BoxProj(l, r) > BoxProj(rt, r) )   /* push far first, near pops first */
          { int t = l; l = rt; rt = t; }
       stack[sp++] = rt;
       stack[sp++] = l;
      }
   }

 return best;
}

/*
 * Occlusion query for Shadow(): is ANYTHING (except the surface itself,
 * r.obj) hit strictly before the light (t*t < ldist2)?
 *
 * Answers the same predicate as LinearIntersect + Shadow's accept rule,
 * but as an ANY-hit walk: no nearest-hit bookkeeping, no near-first
 * child ordering, first qualifying leaf hit returns immediately, and
 * every box test prunes its far slab at t_light = sqrt(ldist2) — half
 * the world never gets entered for a short shadow ray.
 *
 * Bit-identical by construction: the shadow flag is the same function
 * of the same inputs (leaf tests call the same sphere/quad/triangle;
 * the accept rule hit.obj && !=r.obj && hit.t*hit.t < ldist2 mirrors
 * Shadow's '>= ldist2 -> FALSE' rejection, strict as before).
 */
int ShadowOccluded(RAY r, VDBL ldist2)
{
 static int so_no_bvh = -1;
 INTERSECT hit;
 VDBL tlight, tf;
 int stack[64];   /* same depth cap as BVHIntersect: 64 slots always free */
 int sp = 0;

 if( so_no_bvh == -1 )
    so_no_bvh = ( getenv("VRAY_LINEAR") != NULL );

 if( so_no_bvh || nobj <= BVH_MIN_OBJ || ! BVHValid() )
   {
    /* linear path: exactly what Shadow() did through Intersect() */
    int i;
    INTERSECT inter, best;
    best.obj = NULL;
    for( i=0; i<nobj; i++ )
      {
       switch( obj[i].Type )
             {
              case(SPHERE_OBJ):   inter = sphere( &obj[i], r);  break;
              case(PLANE_OBJ):    inter = quad(   &obj[i], r);  break;
              case(TRIANGLE_OBJ): inter = triangle(&obj[i], r); break;
              default:            inter.obj = NULL;
            }
       if( inter.obj && (!best.obj || (inter.t < best.t)) && (inter.obj!=r.obj) )
          best = inter;
      }
    if( best.obj == NULL )
       return 0;
    if( best.t*best.t >= ldist2 )
       return 0;
    return 1;
   }

 tlight = sqrt(ldist2);

 stack[sp++] = bv_root;
 while( sp > 0 )
   {
    BVNode *nd = &bv_node[stack[--sp]];
    tf = tlight;                      /* nothing beyond the light counts */
    if( ! RayBox(&nd->box, r, &tf) )
       continue;

    if( nd->left < 0 )
      {
       int i, e = nd->start + nd->count;
       for( i=nd->start; i<e; i++ )
         {
          OBJ *o = &obj[ bv_perm[i] ];
          switch( o->Type )
                {
                 case(SPHERE_OBJ):
                      hit = sphere(o, r);
                      break;
                 case(PLANE_OBJ):
                      hit = quad(o, r);
                      break;
                 case(TRIANGLE_OBJ):
                      hit = triangle(o, r);
                      break;
                 default:
                      hit.obj = NULL;
                }
          if( hit.obj && (hit.obj != r.obj) && (hit.t*hit.t < ldist2) )
             return 1;          /* ANY qualifying hit occludes: stop */
         }
      }
    else
      {
       stack[sp++] = nd->left;
       stack[sp++] = nd->right;
      }
   }
 return 0;
}
