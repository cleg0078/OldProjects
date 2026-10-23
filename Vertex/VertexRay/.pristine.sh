#!/bin/bash
# .pristine.sh — git-archive HEAD into writable dir, build, gate 13 scenes against the SAME archive's goldens
set -e
D=/home/cleg/MyDev/Vertex/VertexRay/.vrhead
rm -rf $D && mkdir -p $D
git -C /home/cleg/MyDev/Vertex archive HEAD | tar -x -C $D
cd $D/VertexRay
mkdir -p .xgui_pic && cp ../XGUI/*.c ../XGUI/*.h .xgui_pic/
cd .xgui_pic; ls *.c
for f in Button CheckBox Connect Drawing File General Label List Menu Palette Scroll Text Toggle XShell; do gcc -O2 -fPIC -w -c $f.c -I. -I/usr/include/X11; done
ls *.o | wc -l; ar rcs libXGUI_pic.a *.o; cd ..
for f in Mapping Bgcolor Intersect Shade Trace Parse Vector Display BVH; do gcc -O2 -fPIC -w -c $f.c -o $f.o -I. -I.. -I../XGUI -I/usr/include/X11; done
ar rcs libVRay.a Mapping.o Bgcolor.o Intersect.o Shade.o Trace.o Parse.o Vector.o Display.o BVH.o
gcc -O2 VRay.c -o VRay -I. -I../XGUI -I/usr/include/X11 -L. -lVRay -L.xgui_pic -l:libXGUI_pic.a -lX11 -lXext -lm
echo BUILD-OK
# Xvfb
rm -f /tmp/.X99-lock
echo '[]' > $D/egl.json
env __EGL_VENDOR_LIBRARY_FILENAMES=$D/egl.json /home/cleg/MyDev/Vertex/VertexRay/.xvfb/usr/bin/Xvfb :99 -screen 0 1024x768x24 >/dev/null 2>&1 &
XV=$!
sleep 1
export DISPLAY=:99
render_wait() {
  rm -f pop.tga
  ./VRay 3 $1 $2 $3 >/dev/null 2>/dev/null &
  local P=$!
  for i in $(seq 1 15000); do
    SZ=$(stat -c %s pop.tga 2>/dev/null || echo 0)
    [ "$SZ" -eq "$(( $1 * $2 * 3 + 18 ))" ] && { kill $P 2>/dev/null; wait $P 2>/dev/null; return 0; }
    sleep 0.02
  done
  kill $P 2>/dev/null; wait $P 2>/dev/null; return 1
}
for s in ess ess2 ess3 ess5 tri1 tri2 tri2q tri3 tri3q tri4 tri_edge chess; do
  if render_wait 100 100 $s.vr; then
    cmp -s pop.tga tri-goldens/$s.tga && echo "$s MATCH" || echo "$s DIFF"
  else echo "$s TIMEOUT"; fi
done
if render_wait 640 480 jupiter.vr; then
  cmp -s pop.tga tri-goldens/jupiter.tga && echo 'jupiter MATCH' || echo 'jupiter DIFF'
else echo jupiter-TIMEOUT; fi
kill $XV 2>/dev/null; echo DONE