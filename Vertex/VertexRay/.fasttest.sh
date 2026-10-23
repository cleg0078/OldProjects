#!/bin/bash
# .fasttest.sh — self-contained: Xvfb + PIE binary + ess3 render + compare to golden
cd /home/cleg/MyDev/Vertex/VertexRay
# 1. fresh X socket dir
rm -rf /tmp/.X11-unix
mkdir -p /tmp/.X11-unix
# 2. start Xvfb on :99
.xvfb/usr/bin/Xvfb :99 -screen 0 1280x1024x24 -nolisten tcp >/dev/null 2>&1 &
XV=$!
sleep 1
export DISPLAY=:99
SOCK=/tmp/.X11-unix/X99
[ -S $SOCK ] && echo SOCK-OK || { echo SOCK-MISSING; exit 1; }
# 3. render
rm -f pop.tga
./.VRay_PIE 3 100 100 ess3.vr >/dev/null 2>/dev/null &
P=$!
OK=0
for i in $(seq 1 1500); do
  SZ=$(stat -c %s pop.tga 2>/dev/null || echo 0)
  [ $SZ -eq 30018 ] && { OK=1; break; }
  sleep 0.02
done
EPOCH=$(date +%s)
kill $P 2>/dev/null; wait $P 2>/dev/null
if [ $OK -eq 0 ]; then echo NO-RENDER; kill $XV 2>/dev/null; exit 1; fi
cp pop.tga .R2_ess3.tga
cmp -s .R2_ess3.tga .C_ess3.tga && echo FRESH-PIE-MATCHES-GOLDEN || echo FRESH-PIE-DIFFERS
echo epoch=$EPOCH
kill $XV 2>/dev/null