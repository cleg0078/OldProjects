cd /home/cleg/MyDev/Vertex/VertexRay
export __EGL_VENDOR_LIBRARY_FILENAMES=$PWD/.egl_empty.json
rm -rf /tmp/.X11-unix; mkdir -p /tmp/.X11-unix
.xvfb/usr/bin/Xvfb :99 -screen 0 1280x1024x24 -nolisten tcp >/dev/null 2>&1 &
XV=$!; sleep 1; export DISPLAY=:99
J() {
  rm -f pop.tga
  timeout 60 "$1" 3 640 480 jupiter.vr >/dev/null 2>&1
  if [ "$(stat -c %s pop.tga 2>/dev/null || echo 0)" -eq 921618 ]; then cp pop.tga "$2"; else echo "CAPFAIL $2"; fi
}
J "$PWD/VRay" .jn1.tga
J "$PWD/VRay" .jn2.tga
J "$PWD/.vray_head_base" .jh1.tga
J "$PWD/.vray_head_base" .jh2.tga
kill $XV 2>/dev/null
n() { cmp -l "$1" "$2" 2>/dev/null | wc -l; }
a() { cmp -s "$1" "$2" && echo "$3: IDENTICAL" || echo "$3: DIFF $(n "$1" "$2")"; }
a .jn1.tga .jn2.tga "NEW-vs-NEW"
a .jh1.tga .jh2.tga "HEAD-vs-HEAD"
a .jn2.tga .jh2.tga "NEW-vs-HEAD"
exit 0