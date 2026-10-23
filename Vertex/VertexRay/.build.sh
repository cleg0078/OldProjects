#!/bin/bash
# .build.sh — build current disk sources (threaded) -> .new.bin; HEAD sources -> .base2.bin
cd /home/cleg/MyDev/Vertex/VertexRay || exit 1
set -e
CFLAGS="-O2 -fPIC -fcommon -w -I. -I../XGUI -I/usr/include/X11"
gcc $CFLAGS -c VRay.c Parse.c Intersect.c Shade.c Trace.c Mapping.c Bgcolor.c Display.c Vector.c BVH.c
rm -f .new.a; ar rcs .new.a VRay.o Parse.o Intersect.o Shade.o Trace.o Mapping.o Bgcolor.o Display.o Vector.o BVH.o
gcc -O2 VRay.c -o .new.bin $CFLAGS -L. -l:./.new.a -L../XGUI -lXGUI -lX11 -lXext -lm -lpthread
# HEAD baseline
rm -rf .hb && mkdir .hb
(cd /home/cleg/MyDev/Vertex && git archive HEAD VertexRay) | tar -x -C /home/cleg/MyDev/Vertex/VertexRay/.hb
cd .hb/VertexRay
gcc -O2 -fPIC -fcommon -w -I. -I/home/cleg/MyDev/Vertex/XGUI -I/usr/include/X11 -c VRay.c Parse.c Intersect.c Shade.c Trace.c Mapping.c Bgcolor.c Display.c Vector.c BVH.c
rm -f libVRay_h.a; ar rcs libVRay_h.a *.o
gcc -O2 VRay.c -o .base2.bin -O2 -fPIC -fcommon -w -I. -I/home/cleg/MyDev/Vertex/XGUI -I/usr/include/X11 -L. -l:./libVRay_h.a -L/home/cleg/MyDev/Vertex/XGUI -lXGUI -lX11 -lXext -lm -lpthread
cp .base2.bin /home/cleg/MyDev/Vertex/VertexRay/.base2.bin
cd /home/cleg/MyDev/Vertex/VertexRay
echo BUILD-OK
