cd /home/cleg/MyDev/Vertex/VertexRay
cp Shade.c .Shade_new_save.c
git -C /home/cleg/MyDev/Vertex show HEAD:VertexRay/Shade.c > Shade.c
cp VRay ./VRay_new_bak
make all >/dev/null 2>&1; make lib >/dev/null 2>&1; make exe >/dev/null 2>&1
export __EGL_VENDOR_LIBRARY_FILENAMES=$PWD/.egl_empty.json
rm -rf /tmp/.X11-unix; mkdir -p /tmp/.X11-unix
.xvfb/usr/bin/Xvfb :99 -screen 0 1280x1024x24 -nolisten tcp >/dev/null 2>&1 &
XV=$!; sleep 1; export DISPLAY=:99
for i in 1 2 3; do rm -f pop.tga; timeout 60 ./VRay 3 640 480 jupiter.vr >/dev/null 2>&1; echo "jupiter rc=$? size=$(stat -c %s pop.tga 2>/dev/null || echo 0)"; done
for i in 1 2 3; do rm -f pop.tga; timeout 60 ./VRay 3 100 100 chess.vr >/dev/null 2>&1; echo "chess rc=$? size=$(stat -c %s pop.tga 2>/dev/null || echo 0)"; done
cmp -s pop.tga tri-goldens/chess.tga && echo chess-matches-golden || echo chess-differs
kill $XV 2>/dev/null
cp VRay .VRay_headsaved; exit 0