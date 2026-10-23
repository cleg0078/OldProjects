#!/usr/bin/env python3
# bunnygen.py — generates bunny.vr: glass Stanford bunny on a chess board for VertexRay.
# Primitives only: PLANE(quad) + TRIANGLE.  Budget <= 3999 (VRStruct.h MAX_OBJ).
# NOTE: the .vr format has NO comment syntax — this file emits pure scene blocks only.
#
# Source mesh: stanford-bunny.obj (35947 v / 69451 f, Y-up, nose toward -X, ears +Y).
# Pipeline: vertex-cluster decimation (merge-only: quantize on a grid, snap every
# vertex of a cluster to its centroid, drop degenerate/duplicate faces) to a
# ~2600-triangle target, then recenter, 180-deg rotation about Y so the nose faces
# the camera (-Z), scale to 1.6 world units tall, feet just above the tile plane.
#
# Usage: python3 bunnygen.py [grid]   (grid = cluster cell size in source units;
# the source bunny bbox is ~0.156 x 0.154 x 0.121; 0.006..0.008 lands 2-3k tris)
import math, sys

SRC = "stanford-bunny.obj"
OUT = "bunny.vr"
GRID = float(sys.argv[1]) if len(sys.argv) > 1 else 0.0039
HEIGHT = 1.6          # bunny height in world units (~2 board squares)
FEET_Y = 0.004        # board tiles sit at 0.002; float feet just above them
BX, BZ = 0.0, 0.5     # board center under the bunny
NSIGN = 1.0           # world_Z = NSIGN*(srcX-cx): +1 puts the -X nose camera-side; flip if backwards

objs = []
def f(x): return "%.4f" % (x + 0.0)
def v3(t): return " ".join(f(c) for c in t)

MAT = """        AMBIENT < %s >
        DIFFUSE < %s >
        SPECULAR < %s >
        REFLECTION < %4.2f 1.0 1.0 1.0 >
        REFRACTION < 0.0 0.0 0.0 0.0 >
        WIDTH %4.1f
        INDEX 100.0
        REFLEC_DIFFUSE 1.0
        REFRAC_DIFFUSE 1.0
        MAPPING NONE
"""
def mat(amb, dif, spec, refl, width):
    return MAT % (v3(amb), v3(dif), v3(spec), refl, width)

# glass: no diffuse body — color comes from the refraction weight, a reflection
# coat (also fills total-internal-reflection facets, which make Shade() early-return
# losing every term but this one) and a faint cool ambient so nothing goes pure black.
GLASS = """        AMBIENT < 0.0450 0.0650 0.0800 >
        DIFFUSE < 0.0000 0.0000 0.0000 >
        SPECULAR < 0.5000 0.5200 0.5500 >
        REFLECTION < 0.30 1.0 1.0 1.0 >
        REFRACTION < 0.92 0.90 0.95 1.0 >
        WIDTH 60.0
        INDEX 1.5
        REFLEC_DIFFUSE 1.0
        REFRAC_DIFFUSE 1.0
        MAPPING NONE
"""
# polished board: small reflection coat so the glass shows a mirror image
LIGHT_TILE = mat((0.05,0.038,0.025), (0.52,0.40,0.26), (0.10,0.09,0.07), 0.05, 30.0)
DARK_PLATE = mat((0.03,0.018,0.011), (0.17,0.10,0.06), (0.10,0.08,0.06), 0.05, 30.0)
RIM   = mat((0.02,0.012,0.007), (0.11,0.065,0.04), (0.12,0.10,0.08), 0.05, 20.0)
FLOOR = mat((0.16,0.13,0.10), (0.42,0.34,0.27), (0.05,0.05,0.05), 0.0, 8.0)
WALL  = mat((0.16,0.17,0.185), (0.40,0.43,0.48), (0.02,0.02,0.02), 0.0, 8.0)

def tri(p1, p2, p3, m):
    objs.append("TRIANGLE <\n        VERTICES < %s %s %s >\n%s       >\n\n" % (v3(p1), v3(p2), v3(p3), m))
def quad(p1, p2, p3, m):
    objs.append("PLANE <\n        BASE < %s %s %s >\n%s       >\n\n" % (v3(p1), v3(p2), v3(p3), m))

# ---------- load + cluster-decimate the source mesh ----------
V, F = [], []
with open(SRC) as fd:
    for ln in fd:
        if ln.startswith("v "):
            a = ln.split()[1:4]
            V.append((float(a[0]), float(a[1]), float(a[2])))
        elif ln.startswith("f "):
            tok = [t.split("/")[0] for t in ln.split()[1:]]
            if len(tok) != 3:
                raise SystemExit("only triangle faces supported, got: " + ln.strip())
            F.append(tuple(int(t) - 1 for t in tok))

xs = [p[0] for p in V]; ys = [p[1] for p in V]; zs = [p[2] for p in V]
mnx, mny, mnz = min(xs), min(ys), min(zs)
mxx, mxy, mxz = max(xs), max(ys), max(zs)

def key(i):
    x, y, z = V[i]
    return (int((x - mnx) / GRID), int((y - mny) / GRID), int((z - mnz) / GRID))

cell = {}                       # quantized key -> [sumx, sumy, sumz, count]
for i in range(len(V)):
    k = key(i)
    c = cell.get(k)
    if c is None: cell[k] = [V[i][0], V[i][1], V[i][2], 1]
    else: c[0] += V[i][0]; c[1] += V[i][1]; c[2] += V[i][2]; c[3] += 1
keys = list(cell.keys())
key2id = {k: i for i, k in enumerate(keys)}
NV = [(cell[k][0] / cell[k][3], cell[k][1] / cell[k][3], cell[k][2] / cell[k][3]) for k in keys]
KV = [key(i) for i in range(len(V))]

seen, NF = set(), []
for a, b, c in F:
    a, b, c = key2id[KV[a]], key2id[KV[b]], key2id[KV[c]]
    if a == b or b == c or a == c:
        continue
    sig = tuple(sorted((a, b, c)))
    if sig in seen:
        continue
    seen.add(sig)
    NF.append((a, b, c))

# audits: boundary edges (mesh openness) + triangle areas
used = {}
for a, b, c in NF:
    for e in ((a,b),(b,c),(c,a)):
        k = tuple(sorted(e))
        used[k] = used.get(k, 0) + 1
boundary = sum(1 for n in used.values() if n == 1)
areas = []
for a, b, c in NF:
    ux,uy,uz = NV[b][0]-NV[a][0], NV[b][1]-NV[a][1], NV[b][2]-NV[a][2]
    vx,vy,vz = NV[c][0]-NV[a][0], NV[c][1]-NV[a][1], NV[c][2]-NV[a][2]
    nx,ny,nz = uy*vz-uz*vy, uz*vx-ux*vz, ux*vy-uy*vx
    areas.append(0.5*math.sqrt(nx*nx+ny*ny+nz*nz))
# zero-area triangles are rejected by the MINT det-test in Intersect.c -> leaks
tiny = sum(1 for a in areas if a < 1e-9)

# ---------- transform: Y-up source -> Y-up world, nose (src -X) toward camera (-Z) ----------
# 180-deg rotation about Y: worldZ <- srcX, worldX <- -srcZ (proper rotation, det +1)
s = HEIGHT / (mxy - mny)
cx_src = 0.5 * (min(p[0] for p in NV) + max(p[0] for p in NV))
cz_src = 0.5 * (min(p[2] for p in NV) + max(p[2] for p in NV))
def xf(p):
    return (BX - (p[2]-cz_src)*s, FEET_Y + (p[1]-mny)*s, BZ + NSIGN*(p[0]-cx_src)*s)

for a, b, c in NF:
    tri(xf(NV[a]), xf(NV[b]), xf(NV[c]), GLASS)

# ---------- room: floor + wall (wall gives the glass something to refract) ----------
quad((-20.0,-0.05,-20.0),(-20.0,-0.05,20.0),(20.0,-0.05,-20.0),FLOOR)
quad((-20.0,-0.05,12.0),(-20.0,15.0,12.0),(20.0,-0.05,12.0),WALL)

# ---------- board: plate + 32 light tiles + rim (chessgen.py construction) ----------
quad((-4.2,0.0,-4.2),(-4.2,0.0,4.2),(4.2,0.0,-4.2),DARK_PLATE)
for row in range(8):
    for col in range(8):
        if (col + row + 1) % 2 == 1:
            x, z = col-3.5, row-3.5
            quad((x-0.5,0.002,z-0.5),(x-0.5,0.002,z+0.5),(x+0.5,0.002,z-0.5),LIGHT_TILE)
quad((-4.5,0.0,-4.5),(-4.5,0.12,-4.2),(4.5,0.0,-4.5),RIM)
quad((4.5,0.0,4.5),(4.5,0.12,4.2),(-4.5,0.0,4.5),RIM)
quad((-4.5,0.0,4.5),(-4.5,0.12,4.2),(-4.2,0.0,-4.5),RIM)
quad((4.2,0.0,-4.5),(4.2,0.12,-4.2),(4.5,0.0,4.5),RIM)

# ---------- camera + light ----------
POS = (0.0, 1.25, -2.95)
TGT = (0.0, 0.82, 0.5)
d = tuple(t-p for p, t in zip(POS, TGT))
dn = math.sqrt(sum(c*c for c in d))
eye = "EYE <\n     POS < %s %s %s >\n     DIR < %s %s %s >\n     UP < 0.0 1.0 0.3 >\n    >\n\n" % (
      f(POS[0]), f(POS[1]), f(POS[2]), f(d[0]/dn), f(d[1]/dn), f(d[2]/dn))
light = "LIGHT <\n       ORIGIN < -6.0 8.0 -6.0 >\n       ANGLE 2.0\n      >\n\n"
scene = "SCENE <\n       FOV 35.0\n      >\n"

n = len(objs)
assert n <= 12000, "over MAX_OBJ budget: %d" % n
with open(OUT, "w") as fd:
    fd.write("".join(objs) + eye + light + scene)   # NO comments: the .vr parser has none

print("wrote %s | objects: %d / 3999 | bunny tris: %d (from %d) | verts: %d | boundary edges: %d | tiny tris: %d" %
      (OUT, n, len(NF), len(F), len(NV), boundary, tiny))
