#!/bin/bash
# .regr.sh <binary>  -> renders 12 scenes at 100x100 (rec3), copies to .R_<scene>.tga
cd /home/cleg/MyDev/Vertex/VertexRay
BIN=${1:-./VRay}
render_wait() {
  rm -f pop.tga
  $BIN 3 $1 $2 $3 >/dev/null 2>/dev/null &
  local P=$!
  for i in $(seq 1 4000); do
    SZ=$(stat -c %s pop.tga 2>/dev/null || echo 0)
    [ "$SZ" -eq "$(( $1 * $2 * 3 + 18 ))" ] && break
    sleep 0.02
  done
  kill $P 2>/dev/null; wait $P 2>/dev/null
}
for s in ess ess2 ess3 ess5 tri1 tri2 tri2q tri3 tri3q tri4 tri_edge chess; do
  render_wait 100 100 $s.vr && cp pop.tga .R_$s.tga && echo "$s captured"
done
# jupiter 640x480
render_wait 640 480 jupiter.vr && cp pop.tga .R_jupiter.tga && echo 'jupiter captured'