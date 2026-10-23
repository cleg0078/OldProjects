#!/bin/bash
cd /home/cleg/MyDev/Vertex/VertexRay
# Test 1: -O0 vs -O2 same source: does FP-drift change the render?
gcc -O0 -c Shade.c -o .Shade_O0.o -I. -I.. -I../XGUI -I/usr/include/X11 2>/dev/null
cp Shade.o .Shade_O2.o; cp .Shade_O0.o Shade.o
ar -r libVRay.a Mapping.o Bgcolor.o Intersect.o Shade.o Trace.o Parse.o Vector.o Display.o BVH.o >/dev/null 2>&1
gcc -O2 -no-pie VRay.c -o .VRay_O0shade -I. -I../XGUI -I/usr/include/X11 -L. -lVRay -l:libXGUI_copy.a -lX11 -lXext -lm 2>/dev/null
cp .Shade_O2.o Shade.o; ar -r libVRay.a Mapping.o Bgcolor.o Intersect.o Shade.o Trace.o Parse.o Vector.o Display.o BVH.o >/dev/null 2>&1
gcc -O2 -no-pie VRay.c -o VRay -I. -I../XGUI -I/usr/include/X11 -L. -lVRay -l:libXGUI_copy.a -lX11 -lXext -lm 2>/dev/null
render_into() { rm -f pop.tga; $1 3 100 100 ess.vr >/dev/null 2>/dev/null & P=$!; for i in $(seq 1 2000); do SZ=$(stat -c %s pop.tga 2>/dev/null || echo 0); [ "$SZ" -eq 30018 ] && break; sleep 0.02; done; kill $P 2>/dev/null; wait $P 2>/dev/null; cp pop.tga $2; }
render_into ./VRay .R_O2.tga
render_into ./.VRay_O0shade .R_O0.tga
cmp -s .R_O2.tga .R_O0.tga && echo O0-EQ-O2 || python3 - <<'EOF'
from PIL import Image
a=Image.open('.R_O2.tga').convert('RGB'); b=Image.open('.R_O0.tga').convert('RGB')
ap=a.load(); bp=b.load()
d=[(x,y) for y in range(100) for x in range(100) if ap[x,y]!=bp[x,y]]
print('O0 vs O2 diff px:', len(d))
EOF
python3 - <<'EOF'
from PIL import Image
import subprocess
subprocess.run(['cp','.R_O2.tga','tri-goldens/ess.tga'])
EOF
echo ---; echo 'O2 render == committed golden?'; cmp -s .R_O2.tga tri-goldens/ess.tga && echo YES || echo 'no (golden overwritten with O2 capture only if last line printed YES-check-failed)'