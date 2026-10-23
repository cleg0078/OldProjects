cd /home/cleg/MyDev/Vertex/VertexRay
grep -c 'LIGHT <' chess.vr jupiter.vr
echo --- LINEAR chess x2 ---
for i in 1 2; do rm -f pop.tga; VRAY_LINEAR=1 DISPLAY=:0 timeout 60 ./VRay 3 100 100 chess.vr >/dev/null 2>&1; echo "rc=$? size=$(stat -c %s pop.tga 2>/dev/null || echo 0)"; done
echo --- BVH 50x50 chess x3 ---
for i in 1 2 3; do rm -f pop.tga; DISPLAY=:0 timeout 60 ./VRay 3 50 50 chess.vr >/dev/null 2>&1; echo "rc=$? size=$(stat -c %s pop.tga 2>/dev/null || echo 0)"; done