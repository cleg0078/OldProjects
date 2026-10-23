#!/bin/bash
# Line-coverage report for expr.y: builds an instrumented release binary,
# replays all golden + error inputs, runs gcov, and enforces a >=90% gate.

set -e
THRESHOLD=90
COVDIR=$(mktemp -d /tmp/deriv_cov.XXXX)
trap 'rm -rf "$COVDIR"' EXIT

echo "Building instrumented binary..."
bison -d expr.y >/dev/null && flex expr.l >/dev/null
cp expr.tab.c expr.tab.h lex.yy.c expr.y "$COVDIR/"
(
  cd "$COVDIR"
  gcc -O0 --coverage -g -D__RELEASE -c expr.tab.c lex.yy.c 2>/dev/null
  gcc --coverage -o deriv_cov expr.tab.o lex.yy.o -lfl 2>/dev/null
)

echo "Replaying golden inputs..."
grep -v '^#' test_cases.txt | grep '|' | cut -d'|' -f1 | sed 's/^ *//;s/ *$//' | grep -v '^$' \
  | while IFS= read -r line; do printf '%s' "$line" | (cd "$COVDIR" && ./deriv_cov >/dev/null 2>&1 || true); done

echo "Replaying error inputs..."
grep -v '^#' test_errors.txt | grep -v '^[[:space:]]*$' \
  | while IFS= read -r line; do printf '%s' "$line" | (cd "$COVDIR" && ./deriv_cov >/dev/null 2>&1 || true); done

(
  cd "$COVDIR"
  gcov expr.tab.c >/dev/null 2>&1
  PCT=$(gcov expr.tab.c 2>/dev/null | awk "/^File 'expr.y'/{f=1} f && /Lines executed/{gsub(/[^0-9.]/,\"\",\$2); print \$2; exit}")
  echo ""
  echo "expr.y line coverage: ${PCT}%  (gate: >= ${THRESHOLD}%)"
  echo ""
  echo "Uncovered expr.y lines:"
  awk -F: '/#####/{gsub(/ /,"",$2); printf "  %s: ", $2; for(i=3;i<=NF;i++) printf "%s ", $i; printf "\n"}' expr.y.gcov
  echo ""
  echo "Justifications for uncovered lines (identified by rule/function, not line"
  echo "  numbers, which drift as the file is edited):"
  echo "  - 'default:' arms in differentiate()/simplify-arithmetic switch: defensive;"
  echo "    every Node.type enum value is handled explicitly, so they are unreachable"
  echo "    for any AST the parser can produce."
  echo "  - '-0'-as-SUB(0,0) MUL hacks: dead legacy; SUB(NUM,NUM) is now"
  echo "    constant-folded to NUM before these rules can ever match."
  echo "  - 'a*(b*x)' and '(a*x)*b' constant-merge rules: unreachable from"
  echo "    differentiate output because flatten/collect rebuilds products of x as"
  echo "    VAR chains and constant folding catches pure-NUM MULs first."
  echo "  - 'x * -1 = -x' rule: unreachable in practice — differentiate emits the"
  echo "    -1 of the cos rule as the LEFT operand (matched by the '-1 * x' rule),"
  echo "    and a MUL with a -1 RIGHT operand is flattened into the x_power/"
  echo "    coefficient Term structure by flatten_to_terms before simplify_arithmetic"
  echo "    ever inspects it."
  echo "  - 'x^a*x' and 'x^a*x^b' merge rules: unreachable because flatten_to_terms"
  echo "    represents powers of x via x_power counters, so the simplifier never"
  echo "    sees MUL(POW(x,a), x) or MUL(POW,POW) on a plain x base."
  echo "  - 'x^(a+b)' and 'x^(a*1)' NUM-exponent fold rules: these only ever fire on"
  echo "    exponents CONSTRUCTED by the merge rules above (they are the only"
  echo "    producers of ADD/MUL exponents on a plain x base); since those merges"
  echo "    are unreachable, so are these two folds."
  echo "  - 'term_expr->value *= t->coef' in terms_to_expr: constant terms are"
  echo "    rebuilt as NUM(coef), so the wrong-value NUM branch cannot trigger."
  PASS=$(awk -v p="$PCT" -v t="$THRESHOLD" 'BEGIN{print (p+0 >= t+0) ? 1 : 0}')
  [ "$PASS" = "1" ] && { echo ""; echo "Coverage gate PASSED."; exit 0; } \
                    || { echo ""; echo "Coverage gate FAILED."; exit 1; }
)
