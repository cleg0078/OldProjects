#!/usr/bin/env python3
# jupitergen.py - generates jupiter.vr (textured planet + starfield) for VertexRay.
# Uses JupiterMap24.tga (uncompressed 24-bit, desc 0x20): the shipped JupiterMap.tga
# is RLE 32-bit, which the VertexRay TGA loader rejects (silently unmaps).
# NOTE: .vr has NO comment syntax - this file emits pure blocks only.
import math, random
random.seed(7)
NL = chr(10)

PLANET = '''SPHERE <
        CENTER   < 0.0 0.0 0.0 2.0 >
        AMBIENT < 0.045 0.038 0.032 >
        DIFFUSE < 1.0 1.0 1.0 >
        SPECULAR < 0.18 0.18 0.18 >
        REFLECTION < 0.0 1.0 1.0 1.0 >
        REFRACTION < 0.0 0.0 0.0 0.0 >
        WIDTH 30.0
        INDEX 100.0
        REFLEC_DIFFUSE 1.0
        REFRAC_DIFFUSE 1.0
        MAPPING JupiterMap24.tga
       >

'''

STAR = '''SPHERE <
        CENTER   < %s %s %s %s >
        AMBIENT < %s %s %s >
        DIFFUSE < 0.0 0.0 0.0 >
        SPECULAR < 0.0 0.0 0.0 >
        REFLECTION < 0.0 1.0 1.0 1.0 >
        REFRACTION < 0.0 0.0 0.0 0.0 >
        WIDTH 1.0
        INDEX 100.0
        REFLEC_DIFFUSE 1.0
        REFRAC_DIFFUSE 1.0
        MAPPING NONE
       >

'''

def f4(v): return ('%.4f' % (v+0.0))

stars = []
placed = 0
while placed < 220:
    u = random.uniform(-1,1)
    v = random.uniform(-math.pi,math.pi)
    s = math.sqrt(max(0.0,1-u*u))
    dx = s*math.cos(v); dy = u; dz = s*math.sin(v)
    if dz < 0 and abs(dx) < 0.30 and abs(dy) < 0.30:
        continue   # planet silhouette stays clean black against stars
    d = random.uniform(60,95)
    rad = random.uniform(0.045,0.14)   # TRUE radius (post-2026 semantics)
    g = random.uniform(0.55,1.0)
    stars.append(STAR % (f4(dx*d),f4(dy*d),f4(dz*d),f4(rad),f4(g),f4(g),f4(g)))
    placed += 1

EYE = '''EYE <
     POS < 0.0 0.8 9.0 >
     DIR < 0.0 -0.08825 -0.99610 >
     UP < 0.0 1.0 0.0 >
    >

'''

LIGHT = '''LIGHT <
       ORIGIN < -10.0 6.0 8.0 >
       ANGLE 1.0
      >

'''

SCENE = '''SCENE <
       FOV 45.0
      >
'''

open('jupiter.vr','w').write(PLANET + ''.join(stars) + EYE + LIGHT + SCENE)
print('wrote jupiter.vr | objects:', 1+len(stars))
