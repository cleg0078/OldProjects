#!/bin/bash
# usage: .gate.sh   (renders all 13 + jupiter, compares to tri-goldens)
cd /home/cleg/MyDev/Vertex/VertexRay
fail=0
render_wait() {
  local want=$(( $1 * $2 * 3 + 18 ))
  rm -f pop.tga
  ./VRay 3 $1 $2 $3 >/dev/null 2>/dev/null &
  local P=$!
  for i in $(seq 1 4000); do
    SZ=$(stat -c %s pop.tga 2>/dev/null || echo 0)
    [ "$SZ" -eq "$want" ] && break
    sleep 0.02
  done
  kill $P 2>/dev/null; wait $P 2>/dev/null
}
for s in ess ess2 ess3 ess5 tri1 tri2 tri2q tri3 tri3q tri4 tri_edge; do
  render_wait 100 100 $s.vr
  if cmp -s pop.tga tri-goldens/$s.tga; then echo "$s OK"; else echo "$s DIFF"; fail=1; fi
done
render_wait 640 480 jupiter.vr
cmp -s pop.tga tri-goldens/jupiter.tga && echo 'jupiter OK' || { echo jupiter-DIFF; fail=1; }
echo fail=$fail