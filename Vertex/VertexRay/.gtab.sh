cd /home/cleg/MyDev/Vertex/VertexRay
export DISPLAY=:0
for i in 1 2 3 4 5 6 7 8 9 10 11 12; do
gdb -batch -ex run -ex bt --args ./VRay 3 100 100 chess.vr > .gdbout 2>&1
if grep -q SIGSEGV .gdbout; then echo "CRASH attempt $i"; awk "/Program received/{p=1} p" .gdbout | head -20; break; fi
done
grep -c "exited normally" .gdbout || true