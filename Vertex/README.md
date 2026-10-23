# Vertex 3D

A vintage (1995, C.LEGENDRE & S.MAUFROY) X11 3D modeler/renderer: wireframe, flat/Gouraud/Phong shading, a small built-in ray tracer (`VertexRay/`), and a hand-rolled X11 widget toolkit (`XGUI/`). Scene format: binary `.vert` (magic `VT`); can export POV-Ray `.pov`, 3DS-ASCII, and ASCII `.vert`.

## Build

Requires X11 dev headers (libX11, libXext, libXpm). On modern Linux the legacy `/usr/X11` paths in the Makefiles are inert; the `-I/usr/include/X11` flags do the work (`StringDefs.h` lives there — keep that flag).

```sh
make -C XGUI all  && make -C XGUI lib     # 1. widget toolkit  -> XGUI/libXGUI.a
make -C VertexRay all && make -C VertexRay lib   # 2. ray tracer    -> VertexRay/libVRay.a
make all && make lib && make exe          # 3. app: objects -> libVertex.a -> ./Vertex
```

(The `*.a` archives and `*.o` are gitignored build outputs — order matters: XGUI → VertexRay → root.)

Run: `./Vertex` (`-w N -h N -display NAME -color`). Batch ray tracer: `./VertexRay/VRay <recursion> <resx> <resy> <FILE>` (e.g. `./VertexRay/VRay 3 100 100 VertexRay/ess.vr`) writes `pop.tga` into the current directory.

## Layout

- root — the app (models, editing, display, file IO, materials/render dialogs)
- `XGUI/` — widgets (Button/List/Scroll/Text/Menu/File…), toolkit demos: `XGUI/ess.c` etc.
- `VertexRay/` — scanline ray tracer + `.vr` scene parser; `ess*.vr` + `ess.tga`/`xv.tga`/`2.tga` are the smoke-test scene/textures; `tri*.vr` + `tri-goldens/` are the TRIANGLE-primitive validation set; `chess.vr` (439-object chess set, regenerate with `python3 chessgen.py`) uses the raised `MAX_OBJ`
- `obj/` — **sample `.vert` models** (911.vert, dino.vert, …), not build output
- `xbm/` — toolbar icons; `archive/` — dead/orphaned code (DOS `cl.c`, `Xgport.cc`, `try.c`, dead `triangle.c`, stale `MakeRay`, POV-Ray fork `vpov/`); see `archive/` file comments or the plan

## Notes

- `-fcommon` in the root `CC` is load-bearing: globals are defined in `Vertex.h`/`VRay.h` headers and rely on tentative-def merging (pre-gcc-10 default).
- Truecolor (16/24-bit) solid shading and material rendering were broken-by-design (8-bit-only rasterizer, NULL dispatch); fixed — depth-aware rasterizer in `triangle2.c` (included by `VertexSolid.c`). If you find render artifacts.
- **Sphere radius semantics:** the ray-tracer sphere intersect now correctly uses `Radius²` (the 1995 code drew at `√Radius`). Every shipped `ess*.vr` `RADIUS r` was rewritten to `√r` so the shipped renders are unchanged — **new** `.vr` files must specify the true radius.
- **BVH acceleration:** `Intersect()` now walks a bounding-volume hierarchy (`VertexRay/BVH.c` + `BVH.h`) built once at parse end — median split on the longest centroid axis, leaf size 4, iterative slab traversal. Scenes of ≤ 8 objects (or hand-built `obj[]` previews, `VRAY_LINEAR=1` env override) keep the original linear scan, hit semantics identical including the first-in-array tie rule. Measured on the 439-object chess scene: 400×300 2.35 s → 0.22 s (10.5×), 640×480 5.95 s → 0.55 s (10.8×); 221-sphere jupiter.vr 400×400 0.64 s → 0.12 s (5.6×). 12-scene golden regression passes in both modes — BVH and forced-linear renders byte-identical.
- **`MAX_OBJ` raised 300 → 12000:** the chess scene (`VertexRay/chess.vr`, 439 objects) exceeded the 1995 cap; the parser bails `Too many objs` beyond it. Building it recorded two lessons: **the .vr format has no comment syntax** (a stray `#` line prints `Unknow Object: #`, `exit(0)`, leaves a 318-byte 10×10 TGA at rc=0), and `make` has no header deps, so `VRStruct.h` edits need `make clean`.
- **TRIANGLE primitive:** `.vr` gained a third object type — `TRIANGLE < VERTICES < x1 y1 z1 x2 y2 z2 x3 y3 z3 > …material blocks… >`. Intersect is Møller–Trumbore with `quad()`'s parameterization and two-sided normals; texture mapping is the same direction-based planar map as planes, so a triangle coplanar with a quad samples identical texels. Vertices are literal corners (no sphere-style retune). Validation: `triangletest.c` unit harness + `tri*.vr` scenes, goldens in `VertexRay/tri-goldens/`.
- **Multiple lights, colors:** `LIGHT` blocks gained optional `COLOR <r g b>` and `INTENSITY f` (defaults white / 1.0, keywords any order); scenes may carry up to `MAX_LIGHT` = 20 lights, the parser bails `Too many lights` beyond it. Shading loops every light, each with its own shadow ray; `INTENSITY` is a linear multiplier on `COLOR` and lights whose color×intensity sums to ≤ 1e-4 are skipped. A lone white 1.0 light multiplies by exactly 1.0, so all existing goldens stayed byte-identical. Test scenes: `light2.vr` (red key + blue fill), `light3.vr` (mixed keyword order, zero-intensity light), `lights_over.vr` (21 lights → guard). The GUI preview (`VertexMat.c`) uses one explicit white light.
- **Gouraud shading** was dead code (a 1995 debug `return` before the shading loop + an un-wired menu callback); revived & truecolor-safe. The `Gouraud` button now works.
- Safety pass sealed the untrusted-input surface (`.vr`/TGA/`.vert`/LWO bounds, `VertexATOF`, `argv`), `VertexMemory.c` lifecycle (M-01..M-08), and the 40→260 XGUI text buffers. All five shipped scenes re-render pixel-identical to a pre-fix baseline.
