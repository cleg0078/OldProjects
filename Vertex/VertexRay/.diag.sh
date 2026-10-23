#!/bin/bash
cd /home/cleg/MyDev/Vertex/VertexRay
rm -f pop.tga
./VRay 3 100 100 ess.vr >/dev/null 2>/dev/null &
P=$!; for i in $(seq 1 2000); do SZ=$(stat -c %s pop.tga 2>/dev/null || echo 0); [ "$SZ" -eq 30018 ] && break; sleep 0.02; done; kill $P 2>/dev/null; wait $P 2>/dev/null
python3 - <<'EOF'
from PIL import Image
a=Image.open('tri-goldens/ess.tga').convert('RGB'); b=Image.open('pop.tga').convert('RGB')
ap=a.load(); bp=b.load()
W,H=a.size
diff=[(x,y,ap[x,y],bp[x,y]) for y in range(H) for x in range(W) if ap[x,y]!=bp[x,y]]
print('diff px:', len(diff), 'of', W*H)
for d in diff[:8]: print(d)
# max delta and where
m=max(sum(abs(p-q) for p,q in zip(u,v)) for _,_,u,v in diff)
print('max L1 delta:', m)
xs=sorted(set(x for x,_,_,_ in diff)); ys=sorted(set(y for _,y,_,_ in diff))
print('x range', xs[0], xs[-1], 'y range', ys[0], ys[-1])
EOF