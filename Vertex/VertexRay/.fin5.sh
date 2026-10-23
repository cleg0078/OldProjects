cd /home/cleg/MyDev/Vertex/VertexRay
export __EGL_VENDOR_LIBRARY_FILENAMES=$PWD/.egl_empty.json
rm -rf /tmp/.X11-unix; mkdir -p /tmp/.X11-unix
.xvfb/usr/bin/Xvfb :99 -screen 0 1280x1024x24 -nolisten tcp >/dev/null 2>&1 &
XV=$!; sleep 1; export DISPLAY=:99
rm -f pop.tga; timeout 60 ./.vray_head_base 3 640 480 jupiter.vr >/dev/null 2>&1; cp pop.tga .H.tga
rm -f pop.tga; timeout 60 ./VRay 3 640 480 jupiter.vr >/dev/null 2>&1; cp pop.tga .N.tga
kill $XV 2>/dev/null
stat -c "%n %s" .H.tga .N.tga
cmp .H.tga .N.tga | head -2
cmp -l .H.tga .N.tga | head -6
cmp -l .H.tga .N.tga | tail -2
cmp -l .H.tga .N.tga | awk "{r=int((\\$1-19)/1920); c[\\$1>=19?r:0]++} END{for(k in c) print k, c[k]}" | sort -n | head -12
exit 0