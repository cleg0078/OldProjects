#!/bin/bash
cd /home/cleg/MyDev/Vertex/VertexRay
exec > .chess_repro.log 2>&1
pkill -f 'Xvfb :99' 2>/dev/null; sleep 0.3
rm -f /tmp/.X99-lock; echo '[]' > .egl_empty.json
env __EGL_VENDOR_LIBRARY_FILENAMES=$PWD/.egl_empty.json .xvfb/usr/bin/Xvfb :99 -screen 0 1024x768x24 >/dev/null 2>&1 &
XV=$!; sleep 1; export DISPLAY=:99
fails=0
for n in 1 2 3 4 5 6 7 8; do
  rm -f pop.tga
  timeout 15 ./.new.bin 3 100 100 chess.vr 2> .cerr_$n.txt &
  P=$!
  ok=0
  for i in $(seq 1 700); do SZ=$(stat -c %s pop.tga 2>/dev/null || echo 0); [ "$SZ" -eq 30018 ] && { ok=1; break; }; kill -0 $P 2>/dev/null || break; sleep 0.02; done
  kill $P 2>/dev/null; wait $P 2>/dev/null; RC=$?
  if [ $ok -ne 1 ]; then
    fails=$((fails+1))
    echo "run $n FAILED rc=$RC size=$(stat -c %s pop.tga 2>/dev/null || echo none)"
    grep -v 'Parse Obj' .cerr_$n.txt | grep -v 'Sizeof' | head -5
  fi
done
echo "FAILS=$fails/8"
kill $XV 2>/dev/null
echo REPRO-DONE
