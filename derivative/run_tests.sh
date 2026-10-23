#!/bin/bash

# Test runner for derivative calculator
# Builds the release (stdin) binary, then compares actual vs expected output
# for every line of test_cases.txt. Requires bison/flex/gcc.

TEST_FILE="test_cases.txt"
PASS=0
FAIL=0
TOTAL=0

echo "Building release mode (reads expressions from stdin)..."
if ! make -s release >/dev/null; then
    echo "Build failed." >&2
    exit 2
fi

echo "========================================"
echo "Running Derivative Calculator Tests"
echo "========================================"
echo ""

# Process each test line
while IFS='|' read -r input expected || [ -n "$input" ]; do
    # Skip comments and empty lines
    [[ "$input" =~ ^[[:space:]]*# ]] && continue
    [[ -z "${input// }" ]] && continue

    # Trim whitespace (parameter expansion: safe for backslashes/quotes)
    input="${input#"${input%%[![:space:]]*}"}"
    input="${input%"${input##*[![:space:]]}"}"
    expected="${expected#"${expected%%[![:space:]]*}"}"
    expected="${expected%"${expected##*[![:space:]]}"}"

    if [ -z "$input" ] || [ -z "$expected" ]; then
        continue
    fi

    TOTAL=$((TOTAL + 1))

    # Run the derivative and get output
    actual=$(echo "$input" | ./deriv 2>/dev/null | tr -d '\n')
    actual="${actual#"${actual%%[![:space:]]*}"}"
    actual="${actual%"${actual##*[![:space:]]}"}"

    if [ -n "$actual" ] && [ "$actual" = "$expected" ]; then
        echo "[PASS] $input -> $actual"
        PASS=$((PASS + 1))
    else
        echo "[FAIL] $input"
        echo "       expected: $expected"
        echo "       actual:   ${actual:-<no output>}"
        FAIL=$((FAIL + 1))
    fi
done < "$TEST_FILE"

echo ""
echo "========================================"
echo "Results: $PASS passed, $FAIL failed out of $TOTAL tests"
echo "========================================"

if [ $FAIL -eq 0 ]; then
    echo "All tests passed!"
    exit 0
else
    echo "Some tests failed."
    exit 1
fi
