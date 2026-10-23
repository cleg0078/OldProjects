#!/bin/bash
cd /home/cleg/MyDev/Vertex/VertexRay
cp ../XGUI/libXGUI.a .libXGUI_copy.a 2>/dev/null
readelf -h .libXGUI_copy.a 2>/dev/null | grep -o 'Advanced Micro Devices X86-64' | head -1 && echo COPY-IS-X86_64
# test link with the copy
gcc -O2 -no-pie VRay.c -o VRay -I. -I../XGUI -I/usr/include/X11 -L. -l:libXGUI_copy.a -lVRay -lX11 -lXext -lm 2>&1 | head -4
ls -la VRay
./VRay 3 100 100 ess3.vr >/dev/null 2>/dev/null &
P=$!; for i in $(seq 1 500); do [ "$(stat -c %s pop.tga 2>/dev/null || echo 0)" -eq 30018 ] && break; sleep 0.02; done; kill $P 2>/dev/null; wait $P 2>/dev/null
cmp -s pop.tga tri-goldens/ess3.tga && echo ESS3-OK || echo ESS3-DIFF