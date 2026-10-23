#!/bin/bash
cd /home/cleg/MyDev/Vertex/VertexRay
exec > .tsan.log 2>&1
CF="-O1 -g -fPIC -fcommon -w -fsanitize=thread -I. -I../XGUI -I/usr/include/X11"
gcc $CF -c VRay.c Parse.c Intersect.c Shade.c Trace.c Mapping.c Bgcolor.c Display.c Vector.c BVH.c && echo COMPILED
rm -f .tsan.a; ar rcs .tsan.a *.o
gcc -O1 -g -fsanitize=thread VRay.c -o .tsan.bin $CF -L. -l:./.tsan.a -L../XGUI -lXGUI -lX11 -lXext -lm -lpthread && echo LINKED
pkill -f 'Xvfb :99' 2>/dev/null; sleep 0.3
rm -f /tmp/.X99-lock; echo '[]' > .egl_empty.json
env __EGL_VENDOR_LIBRARY_FILENAMES=$PWD/.egl_empty.json .xvfb/usr/bin/Xvfb :99 -screen 0 1024x768x24 >/dev/null 2>&1 &
XV=$!; sleep 1; export DISPLAY=:99
rm -f pop.tga
TSAN_OPTIONS="halt_on_error=0 second_deadlock_stack=1" timeout 120 ./.tsan.bin 1 50 50 chess.vr 2> .tsan_err.txt
RC=$?
kill $XV 2>/dev/null
echo "rc=$RC size=$(stat -c %s pop.tga 2>/dev/null || echo none)"
grep -c 'WARNING: ThreadSanitizer' .tsan_err.txt | head -1
grep -A12 'WARNING: ThreadSanitizer' .tsan_err.txt | head -60
echo TSAN-DONE
