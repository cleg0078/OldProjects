cd /home/cleg/MyDev/Vertex/VertexRay
cp VRay .VRay_new_final; cp .vray_head_base VRay
bash .gate_run.sh > .gate_head.txt 2>&1
mv .VRay_new_final VRay
echo "== GATE ON HEAD BINARY =="; cat .gate_head.txt
exit 0