#!/bin/bash

# Error-path tests: every input in test_errors.txt must exit 1, keep stdout
# empty, and print "Error:" on stderr. Also verifies demo mode (default build).

FAIL=0
TOTAL=0
DEMO_OK=0

echo "Building release mode..."
if ! make -s release >/dev/null; then echo "Build failed." >&2; exit 2; fi

echo "=== error-input tests ==="
while IFS= read -r line || [ -n "$line" ]; do
    line="${line%$'\r'}"
    trimmed="${line#"${line%%[![:space:]]*}"}"
    trimmed="${trimmed%"${trimmed##*[![:space:]]}"}"
    [[ "$trimmed" =~ ^# ]] && continue
    [[ -z "$trimmed" ]] && continue
    TOTAL=$((TOTAL + 1))

    stdout=$(printf '%s' "$line" | ./deriv 2>/tmp/err_stderr.$$); rc=$?
    stderr=$(cat /tmp/err_stderr.$$); rm -f /tmp/err_stderr.$$

    if [ "$rc" -eq 1 ] && [ -z "$stdout" ] && [[ "$stderr" == Error:* ]]; then
        echo "[PASS] $line (rc=1, no stdout, error on stderr)"
    else
        echo "[FAIL] $line rc=$rc stdout=[$stdout] stderr=[$stderr]"
        FAIL=$((FAIL + 1))
    fi
done < test_errors.txt

echo ""
echo "=== empty stdin check ==="
TOTAL=$((TOTAL + 1))
stdout=$(printf '' | ./deriv 2>/tmp/err_stderr.$$); rc=$?
stderr=$(cat /tmp/err_stderr.$$); rm -f /tmp/err_stderr.$$
if [ "$rc" -eq 1 ] && [ -z "$stdout" ] && [[ "$stderr" == Error:* ]]; then
    echo "[PASS] empty stdin (rc=1, no stdout, error on stderr)"
else
    echo "[FAIL] empty stdin rc=$rc stdout=[$stdout] stderr=[$stderr]"
    FAIL=$((FAIL + 1))
fi

echo ""
echo "=== demo-mode test (default, non-release build) ==="
if ! make -s demo >/dev/null; then echo "Demo build failed." >&2; exit 2; fi
demo=$(DERIV_DEMO=1 ./deriv </dev/null); rc=$?
# restore release binary for any subsequent tiers
make -s release >/dev/null
n_solving=$(printf '%s\n' "$demo" | grep -c '^solving ')
if [ "$rc" -eq 0 ] && [ "$n_solving" -eq 8 ] && printf '%s\n' "$demo" | grep -q 'solving sin(x)=cos(x)'; then
    echo "[PASS] demo mode: 8 'solving' lines, exit 0"
    DEMO_OK=1
else
    echo "[FAIL] demo mode: rc=$rc solving_lines=$n_solving"
    FAIL=$((FAIL + 1))
fi

echo ""
echo "Results: $((TOTAL - FAIL + DEMO_OK)) passed, $FAIL failed out of $((TOTAL + 1)) checks"
[ "$FAIL" -eq 0 ] && { echo "All error-path tests passed!"; exit 0; } || { echo "Some tests failed."; exit 1; }
