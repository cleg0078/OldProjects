#!/bin/bash
cd /home/cleg/MyDev/Vertex/VertexRay
t_scene() { # $1=resx $2=resy $3=scene $4=rec $5=env $6=label
  local want=$(( $1 * $2 * 3 + 18 ))
  local best=999999
  for run in 1 2 3; do
    rm -f pop.tga
    local S=$(date +%s%N)
    env $5 ./VRay $4 $1 $2 $3 >/dev/null 2>/dev/null &
    local P=$!
    for i in $(seq 1 3000); do
      SZ=$(stat -c %s pop.tga 2>/dev/null || echo 0)
      [ "$SZ" -eq "$want" ] && break
      sleep 0.02
    done
    local E=$(date +%s%N)
    kill $P 2>/dev/null; wait $P 2>/dev/null
    T=$(( (E-S)/1000000 )); [ $T -lt $best ] && best=$T
  done
  echo "$6: ${best} ms (best of 3)"
}
t_scene 640 480 jupiter.vr 0 "X=1"           "jupiter rec0 BVH   "
t_scene 640 480 jupiter.vr 0 "VRAY_LINEAR=1" "jupiter rec0 linear"
t_scene 640 480 jupiter.vr 2 "X=1"           "jupiter rec2 BVH   "
t_scene 640 480 jupiter.vr 2 "VRAY_LINEAR=1" "jupiter rec2 linear"
t_scene 400 300 chess.vr 3 "X=1"             "chess    rec3 BVH   "
