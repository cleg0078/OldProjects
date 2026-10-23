cd /home/cleg/MyDev/Vertex/VertexRay
export __EGL_VENDOR_LIBRARY_FILENAMES=$PWD/.egl_empty.json
rm -rf /tmp/.X11-unix; mkdir -p /tmp/.X11-unix
.xvfb/usr/bin/Xvfb :99 -screen 0 1280x1024x24 -nolisten tcp >/dev/null 2>&1 &
XV=$!; sleep 1; export DISPLAY=:99
CAP=0
J() { # BIN OUT
  rm -f pop.tga
  timeout 60 "$1" 3 640 480 jupiter.vr >/dev/null 2>&1
  if [ "$(stat -c %s pop.tga 2>/dev/null || echo 0)" -eq 921618 ]; then cp pop.tga "$2"; else echo "CAPFAIL $2"; CAP=1; fi
}
J "$PWD/VRay" .jn1.tga
J "$PWD/VRay" .jn2.tga
J "$PWD/.vray_head_base" .jh1.tga
J "$PWD/.vray_head_base" .jh2.tga
C() { # BIN OUT
  rm -f pop.tga
  timeout 60 "$1" 3 100 100 chess.vr >/dev/null 2>&1
  if [ "$(stat -c %s pop.tga 2>/dev/null || echo 0)" -eq 30018 ]; then cp pop.tga "$2"; else echo "CAPFAIL $2"; CAP=1; fi
}
C "$PWD/VRay" .cn1.tga
C "$PWD/.vray_head_base" .ch1.tga
C "$PWD/VRay" .cn2.tga
C "$PWD/.vray_head_base" .ch2.tga
kill $XV 2>/dev/null
sz() { stat -c %s "$1" 2>/dev/null || echo 0; }
n() { cmp -l "$1" "$2" 2>/dev/null | wc -l; }
if [ $CAP -eq 0 ]; then
  echo "jupiter SAME-BIN new: $(n .jn1.tga .jn2.tga)  head: $(n .jh1.tga .jh2.tga)"
  echo "jupiter CROSS new-head: $(n .jn2.tga .jh2.tga) vs golden: $(n .jn2.tga tri-goldens/jupiter.tga)"
  echo "chess SAME-BIN new: $(n .cn1.tga .cn2.tga)  head: $(n .ch1.tga .ch2.tga)"
  echo "chess CROSS: $(n .cn2.tga .ch2.tga) vs golden: $(n .cn2.tga tri-goldens/chess.tga)"
fi
rm -f .jn1.tga .jn2.tga .jh1.tga .jh2.tga .cn1.tga .cn2.tga .ch1.tga .ch2.tga
exit 0