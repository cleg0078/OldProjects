cd /home/cleg/MyDev/Vertex/VertexRay
export __EGL_VENDOR_LIBRARY_FILENAMES=$PWD/.egl_empty.json
rm -rf /tmp/.X11-unix; mkdir -p /tmp/.X11-unix
.xvfb/usr/bin/Xvfb :99 -screen 0 1280x1024x24 -nolisten tcp >/dev/null 2>&1 &
XV=$!; sleep 1; export DISPLAY=:99
R() {
  BIN=$1; W=$2; H=$3; SC=$4; OUT=$5; TO=$6
  rm -f pop.tga
  timeout $TO "$BIN" 3 "$W" "$H" "$SC" >/dev/null 2>/dev/null
  want=$(( W * H * 3 + 18 ))
  got=$(stat -c %s pop.tga 2>/dev/null || echo 0)
  if [ "$got" -eq "$want" ]; then cp pop.tga "$OUT"; echo "OK $SC"; else echo "CAPFAIL $SC got=$got want=$want"; fi
}
# jupiter + chess parity (both binaries), then light goldens (new only)
R "$PWD/VRay" 640 480 jupiter.vr /home/cleg/MyDev/Vertex/VertexRay/.parity_new_jupiter.tga 60
R "$PWD/.vray_head_base" 640 480 jupiter.vr /home/cleg/MyDev/Vertex/VertexRay/.parity_old_jupiter.tga 60
R "$PWD/VRay" 100 100 chess.vr /home/cleg/MyDev/Vertex/VertexRay/.parity_new_chess.tga 60
R "$PWD/.vray_head_base" 100 100 chess.vr /home/cleg/MyDev/Vertex/VertexRay/.parity_old_chess.tga 60
R "$PWD/VRay" 100 100 light2.vr /home/cleg/MyDev/Vertex/VertexRay/tri-goldens/light2.tga 60
R "$PWD/VRay" 100 100 light3.vr /home/cleg/MyDev/Vertex/VertexRay/tri-goldens/light3.tga 60
kill $XV 2>/dev/null
for s in jupiter chess; do
  N=.parity_new_$s.tga; O=.parity_old_$s.tga
  [ -s "$N" ] && [ -s "$O" ] || { echo "$s MISSING"; continue; }
  cmp -s "$N" "$O" && echo "$s NEWvsHEAD=SAME" || echo "$s NEWvsHEAD=DIFF n=$(cmp -l "$N" "$O" | wc -l)"
done
exit 0