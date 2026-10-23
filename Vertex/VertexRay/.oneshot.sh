#!/bin/bash
# .oneshot.sh — start Xvfb, gate 13 scenes against .C_* committed goldens, kill Xvfb. ONE call.
cd /home/cleg/MyDev/Vertex/VertexRay
rm -f /tmp/.X99-lock
echo '[]' > .egl_empty.json
env __EGL_VENDOR_LIBRARY_FILENAMES=$PWD/.egl_empty.json .xvfb/usr/bin/Xvfb :99 -screen 0 1024x768x24 > .xvfb.log 2>&1 &
XV=$!
sleep 1
for i in 1 2 3 4 5 6 7 8 9 10; do [ -S /tmp/.X11-unix/X99 ] && break; sleep 0.5; done
[ -S /tmp/.X11-unix/X99 ] || { echo NO-SOCKET; kill $XV 2>/dev/null; exit 1; }
export DISPLAY=:99
BIN=${1:-./.base.bin}
tag=${2:-B}
render_wait() {
  rm -f pop.tga
  $BIN 3 $1 $2 $3 >/dev/null 2>/dev/null &
  local P=$!
  local ok=0
  for i in $(seq 1 15000); do
    SZ=$(stat -c %s pop.tga 2>/dev/null || echo 0)
    [ "$SZ" -eq "$(( $1 * $2 * 3 + 18 ))" ] && { ok=1; break; }
    sleep 0.02
  done
  kill $P 2>/dev/null; wait $P 2>/dev/null
  return $((1-ok))
}
fail=0
for s in ess ess2 ess3 ess5 tri1 tri2 tri2q tri3 tri3q tri4 tri_edge chess; do
  if render_wait 100 100 $s.vr; then
    cp pop.tga .R${tag}_$s.tga
    cmp -s .R${tag}_$s.tga tri-goldens/$s.tga && echo "$s MATCH" || echo "$s DIFF"
  else echo "$s TIMEOUT"; fi
done
if render_wait 640 480 jupiter.vr; then
  cp pop.tga .R${tag}_$s.tga; cp pop.tga .R${tag}_jupiter.tga
  cmp -s .R${tag}_jupiter.tga tri-goldens/jupiter.tga && echo 'jupiter MATCH' || echo 'jupiter DIFF'
else echo jupiter-TIMEOUT; fi
kill $XV 2>/dev/null
wait $XV 2>/dev/null
echo DONE