Compute formal derivative of a function

*** NOTES
Results are simplified with a term collector plus algebraic identities
(x^1 -> x, constant folding, 0*x = 0, ...), but are not factorized and
sometimes keep equivalent-but-verbose forms (e.g. (1 + (2 * x)) + 3 * x^2).

*** How to build

1) default build:
 make
 ./deriv          # interactive terminal: runs 8 demo cases
 echo "sin(x)" | ./deriv   # when stdin is piped, reads it like the release build
                          # (force demo even with piped stdin via DERIV_DEMO=1)

2) optimized stdin build:
 make release
 echo "sin(x)" | ./deriv

3) tests:
 make test       # golden suite (test_cases.txt) + error paths (test_errors.txt), pure bash
 make test-all   # + random sympy-oracle battery, then gcov coverage gate (>=90% of expr.y).
                 #   The oracle tier needs python3 + sympy; the Makefile skips it with a
                 #   SKIP notice when python3 is absent (sympy-missing is skipped by the
                 #   script itself). make test stays a pure-bash gate.
 make results    # regenerate TEST_RESULTS.md from a real run of all tiers


JUST A SIMPLE STUDENT PROJECT 

*** REQUIREMENTS ( added in 2025 )
for ubuntu, debian: apt install  bison flex
