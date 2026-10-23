#!/bin/bash
cd /home/cleg/MyDev/Vertex/VertexRay
git -C /home/cleg/MyDev/Vertex show HEAD:VertexRay/tri-goldens/ess3.tga > .g3.tga
rm -f pop.tga
./VRay 3 100 100 ess3.vr >/dev/null 2>/dev/null &
P=$!; for i in $(seq 1 2000); do SZ=$(stat -c %s pop.tga 2>/dev/null || echo 0); [ "$SZ" -eq 30018 ] && break; sleep 0.02; done; kill $P 2>/dev/null; wait $P 2>/dev/null
cmp .g3.tga pop.tga
python3 - <<'EOF'
from PIL import Image
a=Image.open('.g3.tga').convert('RGB'); b=Image.open('pop.tga').convert('RGB')
ap=a.load(); bp=b.load()
D=[(x,y,ap[x,y],bp[x,y]) for y in range(100) for x in range(100) if ap[x,y]!=bp[x,y]]
print('diff count:', len(D))
for d in D[:6]: print(d)
# brightness: is new render uniformly brighter?
import statistics
newb=[sum(bp[x,y]) for x,y,_,_ in D]
oldb=[sum(u) for _,_,u,_ in D]
print('median old:', statistics.median(oldb), 'median new:', statistics.median(newb))
EOF
VRAY_LINEAR=1 ./VRay 3 100 100 ess3.vr >/dev/null 2>/dev/null &
P=$!; for i in $(seq 1 2000); do SZ=$(stat -c %s pop.tga 2>/dev/null || echo 0); [ "$SZ" -eq 30018 ] && break; sleep 0.02; done; kill $P 2>/dev/null; wait $P 2>/dev/null
cmp -s .g3.tga pop.tga && echo LINEAR-MATCHES-GOLDEN || echo LINEAR-ALSO-DIFFERS
python3 - <<'EOF'
from PIL import Image
a=Image.open('.g3.tga').convert('RGB'); b=Image.open('pop.tga').convert('RGB')
ap=a.load(); bp=b.load()
D=[(x,y) for y in range(100) for x in range(100) if ap[x,y]!=bp[x,y]]
print('linear-mode diff count vs golden:', len(D))
EOF