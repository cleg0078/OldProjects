cd /home/cleg/MyDev/Vertex/VertexRay
make all >/dev/null 2>&1; make lib >/dev/null 2>&1; make exe >/dev/null 2>&1
shmd5=$(md5sum VRay | cut -d" " -f1); echo "final exe md5=$shmd5"
export __EGL_VENDOR_LIBRARY_FILENAMES=$PWD/.egl_empty.json
rm -rf /tmp/.X11-unix; mkdir -p /tmp/.X11-unix
.xvfb/usr/bin/Xvfb :99 -screen 0 1280x1024x24 -nolisten tcp >/dev/null 2>&1 &
XV=$!; sleep 1; export DISPLAY=:99
NEWOK=0; NEWBAD=0; NEWVSOLD=0
for i in 1 2 3 4 5; do
  rm -f pop.tga; timeout 60 ./VRay 3 640 480 jupiter.vr >/dev/null 2>&1
  S=$(stat -c %s pop.tga 2>/dev/null || echo 0)
  if [ "$S" -ne 921618 ]; then NEWBAD=$((NEWBAD+1)); echo "new run$i CAP rc=$?"; continue; fi
  NEWOK=$((NEWOK+1)); cp pop.tga .n$i.tga
  if cmp -s pop.tga tri-goldens/jupiter.tga; then echo "new run$i MATCHES-GOLDEN"; else NEWVSOLD=$((NEWVSOLD+1)); echo "new run$i diff-vs-golden=$(cmp -l pop.tga tri-goldens/jupiter.tga | wc -l)"; fi
done
rm -f pop.tga; timeout 60 ./.vray_head_base 3 640 480 jupiter.vr >/dev/null 2>&1
cmp -s pop.tga tri-goldens/jupiter.tga && echo "HEAD MATCHES-GOLDEN" || echo "HEAD diff-vs-golden=$(cmp -l pop.tga tri-goldens/jupiter.tga | wc -l)"
for i in 1 2 3 4 5; do [ -s .n$i.tga ] && for j in $(seq $((i+1)) 5); do [ -s .n$j.tga ] && { cmp -s .n$i.tga .n$j.tga && echo "new$i==new$j" || echo "new$i!=new$j"; }; done; done
kill $XV 2>/dev/null; rm -f .n?.tga
echo "NEW ok=$NEWOK capfail=$NEWBAD vs-golden-diff=$NEWVSOLD"
exit 0