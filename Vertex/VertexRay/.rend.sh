#!/bin/bash
cd /home/cleg/MyDev/Vertex/VertexRay
rm -rf /tmp/.X11-unix; mkdir -p /tmp/.X11-unix
export __EGL_VENDOR_LIBRARY_FILENAMES=$PWD/.egl_empty.json
.xvfb/usr/bin/Xvfb :99 -screen 0 1280x1024x24 -nolisten tcp >/dev/null 2>&1 &
XV=$!; sleep 1
export DISPLAY=:99
[ -S /tmp/.X11-unix/X99 ] && echo SOCK-OK || { echo SOCK-MISSING; exit 1; }
rm -f pop.tga
./VRay $1 $2 $3 $4 >/dev/null 2>/dev/null &
P=$!
OK=0
W=$2; H=$3; BYTES=$(( W * H * 3 + 18 ))
for i in $(seq 1 6000); do
  SZ=$(stat -c %s pop.tga 2>/dev/null || echo 0)
  [ "$SZ" -ge "$BYTES" ] && { OK=1; break; }
  sleep 0.02
done
kill $P 2>/dev/null; wait $P 2>/dev/null
if [ $OK -eq 0 ]; then echo NO-RENDER; kill $XV 2>/dev/null; exit 1; fi
convert pop.tga pop.png && echo RENDER-OK
kill $XV 2>/dev/null
