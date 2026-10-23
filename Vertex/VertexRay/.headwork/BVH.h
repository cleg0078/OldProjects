#ifndef __BVH_H
#define __BVH_H
/* Acceleration structure over obj[0..nobj). See BVH.c for semantics. */
#define BVH_MIN_OBJ 8      /* below this the plain scan is faster */
void BuildBVH(void);       /* build tree for current nobj (no-op if tiny/oversized) */
int  BVHValid(void);       /* 1 if a usable tree matches nobj (rebuilds when stale) */
void BVHInvalidate(void);  /* call after mutating obj[] geometry in place (nobj unchanged): tree rebuilds on next query */
INTERSECT BVHIntersect(RAY r);
#endif
