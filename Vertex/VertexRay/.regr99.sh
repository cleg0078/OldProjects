#!/bin/bash
# .regr99.sh <binary> — 14-scene capture on :99 (Xvfb assumed running), serial (pop.tga is fixed name)
# goldens: the tracked canonical set in tri-goldens/ (regenerated 2026-10-22)
cd /home/cleg/MyDev/Vertex/VertexRay
export DISPLAY=:99
BIN=$1
tag=$2
render_wait() {
  rm -f pop.tga
  $BIN 3 $1 $2 $3 >/dev/null 2>/dev/null &
  local P=$!
  local want=$(( $1 * $2 * 3 + 18 ))
  for i in $(seq 1 30000); do
    SZ=$(stat -c %s pop.tga 2>/dev/null || echo 0)
    [ "$SZ" -eq "$want" ] && break
    sleep 0.02
  done
  kill $P 2>/dev/null; wait $P 2>/dev/null
  # killed-by-SIGTERM makes `wait` return 143 even on a complete render;
  # the size check is the real success test (fix 2026-10-22)
  sleep 0.2
  [ "$(stat -c %s pop.tga 2>/dev/null || echo 0)" -eq "$want" ]
}
fail=0
for s in ess ess2 ess3 ess5 tri1 tri2 tri2q tri3 tri3q tri4 tri_edge chess bunny; do
  render_wait 100 100 $s.vr || { echo "$s TIMEOUT"; fail=1; continue; }
  cp pop.tga .R${tag}_$s.tga
  cmp -s .R${tag}_$s.tga tri-goldens/$s.tga && echo "$s MATCH" || echo "$s DIFF"
done
render_wait 640 480 jupiter.vr || { echo jupiter-TIMEOUT; fail=1; }
cp pop.tga .R${tag}_jupiter.tga
cmp -s .R${tag}_jupiter.tga tri-goldens/jupiter.tga && echo "jupiter MATCH" || echo "jupiter DIFF"
echo fail=$fail