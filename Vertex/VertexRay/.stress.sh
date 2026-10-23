cd /home/cleg/MyDev/Vertex/VertexRay
crash=0; ok=0
for i in $(seq 1 12); do rm -f pop.tga; DISPLAY=:0 timeout 60 ./.vray_head_base 3 100 100 chess.vr >/dev/null 2>&1; rc=$?
  if [ $rc -eq 139 ] || [ $rc -eq 124 ]; then crash=$((crash+1)); else ok=$((ok+1)); fi
done
echo HEAD: ok=$ok bad=$crash
crash=0; ok=0
for i in $(seq 1 12); do rm -f pop.tga; DISPLAY=:0 timeout 60 ./VRay 3 100 100 chess.vr >/dev/null 2>&1; rc=$?
  if [ $rc -eq 139 ] || [ $rc -eq 124 ]; then crash=$((crash+1)); else ok=$((ok+1)); fi
done
echo NEW: ok=$ok bad=$crash