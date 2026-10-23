#!/usr/bin/env python3
"""Random-expression oracle for the derivative calculator.

Generates deterministic (seeded) random valid expressions, computes the
reference derivative with sympy, and checks the program's printed result
numerically at several sample points. The printed format is parsed by a
built-in evaluator whose '^' is LEFT-associative to match the grammar.

Usage: python3 tests/oracle_check.py [N]     (default N=200)
Requires python3 + sympy; skips gracefully if sympy is missing.
"""
import random
import subprocess
import sys

try:
    from sympy import (Symbol, cos, diff, exp, log, sin, tan, simplify)
except ImportError:
    print("SKIP: python3-sympy not installed; skipping oracle tests")
    sys.exit(0)

x = Symbol('x')

# ---------- expression generation (AST tuples) ----------
FUNCS = {'sin', 'cos', 'tan', 'exp', 'ln', 'log'}
FUNCS_SY = {'sin': sin, 'cos': cos, 'tan': tan, 'exp': exp,
            'ln': log, 'log': lambda a: log(a, 10)}

def gen(rng, depth):
    r = rng.random()
    if depth <= 0 or r < 0.35:
        if rng.random() < 0.4:
            return ('x',)
        return ('n', rng.randint(1, 5))
    if r < 0.5:
        f = rng.choice(list(FUNCS))
        return ('f', f, gen(rng, depth - 1))
    if r < 0.75:
        op = rng.choice(['+', '-', '*', '/'])
        return ('b', op, gen(rng, depth - 1), gen(rng, depth - 1))
    # power: base x or small expr; exponent small int or x-expr
    base = ('x',) if rng.random() < 0.6 else ('n', rng.randint(2, 3))
    if rng.random() < 0.7:
        return ('p', base, ('n', rng.randint(2, 4)))
    return ('p', base, ('x',))

def to_prog(a):
    kind = a[0]
    if kind == 'x': return 'x'
    if kind == 'n': return str(a[1])
    if kind == 'f': return f'{a[1]}({to_prog(a[2])})'
    if kind == 'b': return f'({to_prog(a[2])} {a[1]} {to_prog(a[3])})'
    if kind == 'p':
        b, e = to_prog(a[1]), to_prog(a[2])
        return f'{b}^{e}'
    raise ValueError(a)

def to_sympy(a):
    kind = a[0]
    if kind == 'x': return x
    if kind == 'n': return a[1]
    if kind == 'f': return FUNCS_SY[a[1]](to_sympy(a[2]))
    if kind == 'b':
        l, r = to_sympy(a[2]), to_sympy(a[3])
        return {'+': lambda: l + r, '-': lambda: l - r,
                '*': lambda: l * r, '/': lambda: l / r}[a[1]]()
    if kind == 'p': return to_sympy(a[1]) ** to_sympy(a[2])

# ---------- program output evaluation (left-assoc ^) ----------
import math
import re as _re

FUNCS_PY = None  # defined below before use

TOKENS = _re.compile(r'\s*(\d+|sin|cos|tan|exp|ln|log|x|[()+\-*/^])')

class Ev:
    def __init__(self, s):
        self.t = []
        pos = 0
        while pos < len(s):
            m = TOKENS.match(s, pos)
            if not m:
                raise ValueError(f'bad char at {pos}: {s[pos:]}')
            self.t.append(m.group(1)); pos = m.end()
        self.i = 0
    def peek(self):
        return self.t[self.i] if self.i < len(self.t) else None
    def eat(self, w=None):
        tok = self.peek()
        if w and tok != w:
            raise ValueError(f'expected {w} got {tok}')
        self.i += 1
        return tok
    def expr(self):
        v = self.term()
        while self.peek() in ('+', '-'):
            op = self.eat()
            r = self.term()
            v = v + r if op == '+' else v - r
        return v
    def term(self):
        v = self.pow_()
        while self.peek() in ('*', '/'):
            op = self.eat()
            r = self.pow_()
            v = v * r if op == '*' else v / r
        return v
    def pow_(self):
        v = self.unary()
        while self.peek() == '^':
            self.eat()
            v = v ** self.unary()   # left-assoc, matching the grammar
        return v
    def unary(self):
        if self.peek() == '-':
            self.eat()
            return -self.unary()
        return self.prim()
    def prim(self):
        tok = self.peek()
        if tok == '(':
            self.eat(); v = self.expr(); self.eat(')'); return v
        if tok == 'x':
            self.eat(); return math.nan  # placeholder, substituted below
        if tok in FUNCS:
            self.eat(); self.eat('('); v = self.expr(); self.eat(')')
            return FUNCS_PY[tok](v)
        self.eat(); return float(tok)

def eval_out(s, xv):
    global XVAL
    XVAL = xv
    return Ev(s).expr()

# monkeypatch x token
_orig_prim = Ev.prim
def prim(self):
    tok = self.peek()
    if tok == 'x':
        self.eat()
        return XVAL
    return _orig_prim(self)
Ev.prim = prim

FUNCS_PY = {'sin': math.sin, 'cos': math.cos, 'tan': math.tan, 'exp': math.exp,
            'ln': math.log, 'log': lambda a: math.log(a, 10)}

POINTS = [0.31, 0.62, 0.9, 1.1, 1.4, 1.8, 2.3, 2.7]

def main():
    n = int(sys.argv[1]) if len(sys.argv) > 1 else 200
    rng = random.Random(42)
    tested = skipped = failed = 0
    for _ in range(n):
        ast = gen(rng, 3)
        inp = to_prog(ast)
        try:
            ref = diff(to_sympy(ast), x)
        except Exception:
            skipped += 1
            continue
        p = subprocess.run(['./deriv'], input=inp, capture_output=True, text=True, timeout=15)
        out = p.stdout.strip()
        if p.returncode != 0 or not out:
            print(f'FAIL {inp}\n  rc={p.returncode} out={out!r} err={p.stderr.strip()[:80]}')
            failed += 1
            continue
        agree = tot = 0
        for p_ in POINTS:
            try:
                got = eval_out(out, p_)
                rv = complex(ref.subs(x, p_))
                gv = complex(got)
                if abs(rv) > 1e10 or abs(gv) > 1e10 or abs(rv.imag) > 1e-6 or abs(gv.imag) > 1e-6:
                    continue
                err = abs(rv - gv) / max(1.0, abs(rv))
            except Exception:
                continue
            tot += 1
            if err < 1e-6:
                agree += 1
        if tot < 3:
            skipped += 1
            continue
        if agree == tot:
            tested += 1
        else:
            failed += 1
            print(f'FAIL {inp}\n  out={out}\n  ref={simplify(ref)}\n  {agree}/{tot} points agree')
    print(f'\noracle: {tested} verified, {skipped} skipped (domain), {failed} FAILED')
    sys.exit(1 if failed else 0)

if __name__ == '__main__':
    main()
