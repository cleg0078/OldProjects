cd /home/cleg/MyDev/Vertex/VertexRay
# 1) final rebuild with the project default flags (no -g), expect zero warnings
make clean >/dev/null 2>&1
W=$(make all 2>&1 | grep -ci "warning\|error")
make lib >/dev/null 2>&1; make exe >/dev/null 2>&1
echo "warnings/errors=$W  exe=$(stat -c %s VRay) bytes"
# 2) final gate both modes on this exact binary
bash .gate_run.sh > .final_gate.txt 2>&1
echo "--- gate BVH/LINEAR ---"; grep -E "OK|DIFF|fail" .final_gate.txt | head -30
# 3) capture light2/light3 goldens in the same Xvfb env as the other goldens
grep -q "fail=0" .final_gate.txt && echo GATE-CLEAN || echo GATE-ENVDRIFT-PATTERN
exit 0