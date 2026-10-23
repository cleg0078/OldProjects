#!/usr/bin/env python3
# chessgen.py — generates chess.vr: full Staunton-style chess set for VertexRay.
# Primitives only: SPHERE + PLANE(quad) + TRIANGLE.  Budget <= 3999 (VRStruct.h MAX_OBJ).
# NOTE: the .vr format has NO comment syntax — this file emits pure scene blocks only.
# Height table (multiples of one square, tournament Staunton):
#   king 2.0 | queen 1.7 | bishop 1.4 | knight 1.2 | rook 1.0 | pawn 0.8
# Render:  ./VRay 3 640 480 chess.vr
import math

OUT = "chess.vr"
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

IVORY = mat((0.22,0.19,0.155), (0.62,0.55,0.42), (0.30,0.28,0.25), 0.06, 12.0)
EBONY = mat((0.16,0.115,0.095), (0.30,0.22,0.18), (0.45,0.40,0.36), 0.10, 14.0)
LIGHT_TILE = mat((0.05,0.038,0.025), (0.52,0.40,0.26), (0.10,0.09,0.07), 0.0, 8.0)
DARK_PLATE = mat((0.03,0.018,0.011), (0.17,0.10,0.06), (0.10,0.08,0.06), 0.0, 8.0)
RIM   = mat((0.02,0.012,0.007), (0.11,0.065,0.04), (0.12,0.10,0.08), 0.05, 20.0)
FLOOR = mat((0.09,0.075,0.06), (0.22,0.18,0.15), (0.05,0.05,0.05), 0.0, 8.0)
WALL  = mat((0.08,0.065,0.055), (0.16,0.13,0.11), (0.02,0.02,0.02), 0.0, 8.0)
def hmat(side): return IVORY if side == "w" else EBONY

def sph(c, r, m): objs.append("SPHERE <\n        CENTER < %s %s >\n%s       >\n\n" % (v3(c), f(r), m))
def quad(p1, p2, p3, m): objs.append("PLANE <\n        BASE < %s %s %s >\n%s       >\n\n" % (v3(p1), v3(p2), v3(p3), m))
def tri(p1, p2, p3, m): objs.append("TRIANGLE <\n        VERTICES < %s %s %s >\n%s       >\n\n" % (v3(p1), v3(p2), v3(p3), m))

def collar(cx, cz, y_top, y_bot, s, m):
    """4 chamfer TRIANGLEs forming a bevel ring under a collar — the TRIANGLE showcase."""
    a=(cx-s,y_top,cz-s); b=(cx+s,y_top,cz-s); c=(cx+s,y_top,cz+s); d=(cx-s,y_top,cz+s)
    tri(a,b,(cx-s,y_bot,cz-s),m); tri(b,c,(cx+s,y_bot,cz-s),m)
    tri(c,d,(cx+s,y_bot,cz+s),m); tri(d,a,(cx-s,y_bot,cz+s),m)

def base_prism(cx, cz, side, h=0.15, s=0.30):
    m = hmat(side)
    x0,x1,z0,z1 = cx-s, cx+s, cz-s, cz+s
    quad((x0,0,z0),(x0,h,z0),(x0,0,z1),m)
    quad((x1,0,z1),(x1,h,z1),(x1,0,z0),m)
    quad((x0,0,z1),(x0,h,z1),(x1,0,z1),m)
    quad((x1,0,z0),(x1,h,z0),(x0,0,z0),m)
    quad((x0,h,z0),(x0,h,z1),(x1,h,z0),m)

def pawn(cx, cz, side):
    m = hmat(side)
    sph((cx,0.45,cz),0.32,m)                      # body
    collar(cx,cz,0.60,0.46,0.26,m)                # collar + 4 TRIANGLEs
    sph((cx,0.72,cz),0.13,m)                      # head

def rook(cx, cz, side):
    m = hmat(side)
    sph((cx,0.42,cz),0.24,m)                      # column
    quad((cx-0.26,0.62,cz-0.26),(cx-0.26,0.92,cz-0.26),(cx-0.26,0.62,cz+0.26),m)
    quad((cx+0.26,0.62,cz+0.26),(cx+0.26,0.92,cz+0.26),(cx+0.26,0.62,cz-0.26),m)
    quad((cx-0.26,0.62,cz+0.26),(cx-0.26,0.92,cz+0.26),(cx+0.26,0.62,cz+0.26),m)
    quad((cx+0.26,0.62,cz-0.26),(cx+0.26,0.92,cz-0.26),(cx-0.26,0.62,cz-0.26),m)
    quad((cx-0.26,0.92,cz-0.26),(cx-0.26,0.92,cz+0.26),(cx+0.26,0.92,cz-0.26),m)   # keep top
    for sx in (-0.20,0.20):
        for sz in (-0.20,0.20):
            sph((cx+sx,0.97,cz+sz),0.07,m)         # 4 crenels

def knight(cx, cz, side):
    m = hmat(side)
    d = 1 if side == "w" else -1
    sph((cx,0.38,cz),0.22,m)                       # neck
    sph((cx,0.55,cz-0.16*d),0.14,m)                # chest
    sph((cx,0.72,cz+0.14*d),0.20,m)                # head
    sph((cx,0.60,cz+0.34*d),0.11,m)                # muzzle
    sph((cx-0.10,0.92,cz+0.06*d),0.07,m)           # ears
    sph((cx+0.10,0.92,cz+0.06*d),0.07,m)
    sph((cx,0.80,cz-0.16*d),0.09,m)                # mane

def bishop(cx, cz, side):
    m = hmat(side)
    sph((cx,0.48,cz),0.22,m)                       # body
    sph((cx,0.98,cz),0.20,m)                       # mitre
    quad((cx-0.04,0.84,cz+0.201),(cx+0.04,0.84,cz+0.201),(cx-0.04,1.24,cz+0.201),m)  # mitre slit
    collar(cx,cz,0.66,0.52,0.22,m)                 # collar + 4 TRIANGLEs

def queen(cx, cz, side):
    m = hmat(side)
    sph((cx,0.48,cz),0.24,m)                       # body
    collar(cx,cz,0.72,0.56,0.28,m)                 # collar + 4 TRIANGLEs
    sph((cx,1.00,cz),0.22,m)                       # crown
    for sx in (-0.21,0.21):
        for sz in (-0.21,0.21):
            sph((cx+sx,1.14,cz+sz),0.09,m)          # crown orbs
    sph((cx,1.42,cz),0.09,m)                       # top pearl
    quad((cx-0.02,1.24,cz),(cx+0.02,1.24,cz),(cx-0.02,1.70,cz),m)   # spike

def king(cx, cz, side):
    m = hmat(side)
    sph((cx,0.50,cz),0.26,m)                       # body
    collar(cx,cz,0.78,0.62,0.30,m)                 # collar + 4 TRIANGLEs
    sph((cx,1.12,cz),0.25,m)                       # crown
    for sx,sz in ((-0.25,0.0),(0.25,0.0),(0.0,-0.25),(0.0,0.25)):
        sph((cx+sx,1.28,cz+sz),0.08,m)              # crown orbs
    quad((cx-0.07,1.35,cz+0.002),(cx+0.07,1.35,cz+0.002),(cx-0.07,2.00,cz+0.002),m)  # cross upright
    quad((cx-0.28,1.56,cz+0.004),(cx+0.28,1.56,cz+0.004),(cx-0.28,1.72,cz+0.004),m)  # cross arm

# ---------- room: floor + wall ----------
quad((-20.0,-0.05,-20.0),(-20.0,-0.05,20.0),(20.0,-0.05,-20.0),FLOOR)
quad((-20.0,-0.05,12.0),(-20.0,15.0,12.0),(20.0,-0.05,12.0),WALL)

# ---------- board: plate + 32 light tiles + rim ----------
quad((-4.2,0.0,-4.2),(-4.2,0.0,4.2),(4.2,0.0,-4.2),DARK_PLATE)
for row in range(8):
    for col in range(8):
        if (col + row + 1) % 2 == 1:
            x, z = col-3.5, row-3.5
            quad((x-0.5,0.002,z-0.5),(x-0.5,0.002,z+0.5),(x+0.5,0.002,z-0.5),LIGHT_TILE)
quad((-4.5,0.0,-4.5),(-4.5,0.12,-4.2),(4.5,0.0,-4.5),RIM)         # 4 rim rails
quad((4.5,0.0,4.5),(4.5,0.12,4.2),(-4.5,0.0,4.5),RIM)
quad((-4.5,0.0,4.5),(-4.5,0.12,4.2),(-4.2,0.0,-4.5),RIM)
quad((4.2,0.0,-4.5),(4.2,0.12,-4.2),(4.5,0.0,4.5),RIM)

# ---------- pieces: tournament start array (white near camera at -z) ----------
BACK = "RNBQKBNR"
for col, piece in enumerate(BACK):
    x = col - 3.5
    base_prism(x,-3.5,"w"); {"P":pawn,"R":rook,"N":knight,"B":bishop,"Q":queen,"K":king}[piece](x,-3.5,"w")
    base_prism(x, 3.5,"b"); {"P":pawn,"R":rook,"N":knight,"B":bishop,"Q":queen,"K":king}[piece](x, 3.5,"b")
for col in range(8):
    x = col - 3.5
    base_prism(x,-2.5,"w"); pawn(x,-2.5,"w")
    base_prism(x, 2.5,"b"); pawn(x, 2.5,"b")

n = len(objs)
assert n <= 3999, "over MAX_OBJ budget: %d" % n

tx,ty,tz = 0.0-0.0, 0.5-6.2, 0.0-(-10.5)
dn = math.sqrt(tx*tx+ty*ty+tz*tz)
eye   = "EYE <\n     POS < 0.0 6.2 -10.5 >\n     DIR < 0.0 %s %s >\n     UP < 0.0 1.0 0.4 >\n    >\n\n" % (f(ty/dn), f(tz/dn))
light = "LIGHT <\n       ORIGIN < 9.0 9.0 -2.0 >\n       ANGLE 2.0\n      >\n\n"
scene = "SCENE <\n       FOV 48.0\n      >\n"

with open(OUT,"w") as fd:
    fd.write("".join(objs) + eye + light + scene)   # NO comments: the .vr parser has none

bytype={}
for o in objs: bytype[o.split(" <")[0]] = bytype.get(o.split(" <")[0],0)+1
print("wrote",OUT,"| objects:",n,"/ 3999 |",bytype)
