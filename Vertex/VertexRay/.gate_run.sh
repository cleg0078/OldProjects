cd /home/cleg/MyDev/Vertex/VertexRay
export __EGL_VENDOR_LIBRARY_FILENAMES=$PWD/.egl_empty.json
rm -rf /tmp/.X11-unix; mkdir -p /tmp/.X11-unix
.xvfb/usr/bin/Xvfb :99 -screen 0 1280x1024x24 -nolisten tcp >/dev/null 2>&1 &
XV=$!; sleep 1
export DISPLAY=:99
echo "== gate BVH =="
bash .gate.sh
echo "== gate LINEAR =="
VRAY_LINEAR=1 bash .gate.sh
kill $XV 2>/dev/null
exit 0