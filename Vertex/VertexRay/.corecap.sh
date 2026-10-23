cd /home/cleg/MyDev/Vertex/VertexRay
cat /proc/sys/kernel/core_pattern
ulimit -c unlimited
for i in 1 2 3 4 5 6; do rm -f pop.tga core
DISPLAY=:0 timeout 60 ./VRay 3 100 100 chess.vr >/dev/null 2>&1
rc=$?
if [ $rc -eq 139 ]; then echo CRASH on attempt $i; ls -la core 2>/dev/null; break; fi
echo ok$i rc=$rc
done
which coredumpctl && coredumpctl list --no-pager 2>/dev/null | tail -3