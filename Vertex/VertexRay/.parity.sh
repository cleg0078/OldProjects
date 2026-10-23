#!/bin/bash
cd /home/cleg/MyDev/Vertex/VertexRay || exit 1
HB=./.vray_head_base
export DISPLAY=:0
R() {
  BIN=$1; W=$2; H=$3; SC=$4; OUT=$5; TO=$6
  rm -f pop.tga
  timeout $TO "$BIN" 3 "$W" "$H" "$SC" >/dev/null 2>/dev/null
  want=$(( W * H * 3 + 18 ))
  got=$(stat -c %s pop.tga 2>/dev/null || echo 0)
  if [ "$got" -eq "$want" ]; then cp pop.tga "$OUT"; else echo "CAPFAIL $SC bin=$BIN got=$got want=$want"; fi
}
for s in ess ess2 ess3 ess5 tri1 tri2 tri2q tri3 tri3q tri4 tri_edge chess; do
  R ./VRay 100 100 "$s.vr" "./.parity_new_$s.tga" 25
  R "$HB"  100 100 "$s.vr" "./.parity_old_$s.tga" 25
done
R ./VRay 640 480 jupiter.vr ./.parity_new_jupiter.tga 60
R "$HB"  640 480 jupiter.vr ./.parity_old_jupiter.tga 60
R ./VRay 100 100 light2.vr ./.parity_l2a.tga 25
R ./VRay 100 100 light2.vr ./.parity_l2b.tga 25
R ./VRay 100 100 light3.vr ./.parity_l3.tga 25
echo "=== PARITY RESULTS ==="
P=0
for s in ess ess2 ess3 ess5 tri1 tri2 tri2q tri3 tri3q tri4 tri_edge chess jupiter; do
  N=./.parity_new_$s.tga; O=./.parity_old_$s.tga
  [ -s "$N" ] && [ -s "$O" ] || { echo "$s MISSING"; P=1; continue; }
  if cmp -s "$N" "$O"; then r1=SAME; else r1="DIFF(n=$(cmp -l "$N" "$O" | wc -l))"; P=1; fi
  echo "$s NEWvsHEAD=$r1"
done
cmp -s ./.parity_l2a.tga ./.parity_l2b.tga && echo "light2 DETERMINISTIC" || echo "light2 NONDET-OR-MISSING"
echo "light3: $(stat -c %s ./.parity_l3.tga 2>/dev/null || echo MISSING) bytes (want 30018)"
echo parity_fail=$P
cd /home/cleg/MyDev/Vertex
timeout 5 ./Vertex >/dev/null 2>/dev/null; echo "GUI smoke rc=$? (want 124)"
exit 0