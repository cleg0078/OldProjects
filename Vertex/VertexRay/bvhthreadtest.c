/* Headless BVH startup regression (white-box: include BVH.c to inspect state).
 * Build after make lib:
 * gcc -O2 -g -I. -I../XGUI bvhthreadtest.c -o /tmp/bvhthreadtest \
 *     -L. -L../XGUI -lVRay -lXGUI -lX11 -lXext -lm -pthread
 * Run: VRAY_THREADS=64 /tmp/bvhthreadtest chess.vr
 */
#include "VRay.h"
#include "BVH.c"
#include <assert.h>
#include <stdint.h>

static uint64_t digest;
static int pixels;
static void pixel(int x, int y, unsigned long r, unsigned long g, unsigned long b)
{
 assert(x == pixels % xres && y == pixels / xres);
 digest = (digest ^ (unsigned char)r) * UINT64_C(1099511628211);
 digest = (digest ^ (unsigned char)g) * UINT64_C(1099511628211);
 digest = (digest ^ (unsigned char)b) * UINT64_C(1099511628211);
 pixels++;
}

static int check_node(int node, int depth)
{
 BVNode *nd;
 assert(node >= 0 && node < bv_node_count);
 assert(depth <= BV_MAX_DEPTH);
 nd = &bv_node[node];
 if(nd->left < 0) {
  assert(nd->start >= 0 && nd->count > 0);
  assert(nd->start + nd->count <= nobj);
  return nd->count;
 }
 /* Construction allocates parents before children: no cycles possible. */
 assert(nd->left > node && nd->right > node);
 return check_node(nd->left, depth+1) + check_node(nd->right, depth+1);
}

int main(int argc, char **argv)
{
 int i;
 uint64_t first = 0;
 assert(argc == 2);
 ParseFile(argv[1]);
 assert(nobj > BVH_MIN_OBJ);
 /* Must already be built on return, without triggering a lazy rebuild. */
 assert(bv_built_nobj == nobj && !bv_stale);
 assert(check_node(bv_root, 0) == nobj);
 AddPixel = pixel;
 xres = yres = 100;
 maxrec_level = 3;
 for(i=0; i<10; i++) {
  /* Exercise render startup with an invalid tree, as embedded callers do. */
  BVHInvalidate();
  digest = UINT64_C(14695981039346656037);
  pixels = 0;
  RayTrace(NULL, 0);
  assert(pixels == xres*yres && bv_built_nobj == nobj && !bv_stale);
  assert(check_node(bv_root, 0) == nobj);
  if(i == 0) first = digest;
  assert(digest == first);
 }
 printf("PASS: eager parse build, invalidated render startup, 10 renders; hash=%016llx\n",
        (unsigned long long)first);
 return 0;
}
