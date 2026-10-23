cd /home/cleg/MyDev/Vertex/VertexRay
head_ok=0; head_bad=0
for i in $(seq 1 10); do rm -f pop.tga; DISPLAY=:0 timeout 45 ./.vray_head_base 3 100 100 chess.vr >/dev/null 2>&1; rc=$?
  if [ $rc -eq 139 ] || [ $rc -eq 124 ]; then head_bad=$((head_bad+1)); else head_ok=$((head_ok+1)); fi
done
echo HEAD: ok=$head_ok bad=$head_bad
exit 0