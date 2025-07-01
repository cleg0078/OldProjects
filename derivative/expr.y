%code requires {
#ifndef NODE_DEFINED
#define NODE_DEFINED
typedef struct Node {
    enum { NUM, VAR, ADD, SUB, MUL, DIV, NODE_SIN, NODE_COS, NODE_TAN, NODE_EXP, NODE_LN } type;
    struct Node *left, *right;
    int value;
} Node;
#endif
}

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef NODE_DEFINED
#define NODE_DEFINED
typedef struct Node {
    enum { NUM, VAR, ADD, SUB, MUL, DIV, NODE_SIN, NODE_COS, NODE_TAN, NODE_EXP, NODE_LN } type;
    struct Node *left, *right;
    int value;
} Node;
#endif

Node* make_node(int type, Node* left, Node* right, int value);
Node* differentiate(Node* expr);
Node* simplify(Node* expr);
char* print_to_string(Node* e);
void print_expr(Node* expr);
void free_expr(Node* expr);

int yylex(void);
void yyerror(const char *s);
%}


%union {
    int num;
    Node* node;
}
%start input

%token <num> NUMBER
%token VARIABLE
%token SIN COS TAN EXP LN
%type <node> expr func


%left '+' '-'
%left '*' '/'
%right UMINUS

%%

input:
     expr {
	Node* d = differentiate($1);
	Node* s = simplify(d);
	print_expr(s); printf("\n");
	free_expr($1); 
	free_expr(d); 
	free_expr(s);
    }
;

func
    : SIN '(' expr ')'    { $$ = make_node(NODE_SIN, $3, NULL, 0); }
    | TAN '(' expr ')'    { $$ = make_node(NODE_TAN, $3, NULL, 0); }
    | EXP '(' expr ')'    { $$ = make_node(NODE_EXP, $3, NULL, 0); }
    | COS '(' expr ')'    { $$ = make_node(NODE_COS, $3, NULL, 0); }
    | LN '(' expr ')'    { $$ = make_node(NODE_LN, $3, NULL, 0); }

expr:
    NUMBER           { $$ = make_node(NUM, NULL, NULL, $1); }
    | VARIABLE         { $$ = make_node(VAR, NULL, NULL, 0); }
    | func             { $$ = $1; }
    | expr '+' expr    { $$ = make_node(ADD, $1, $3, 0); }
    | expr '-' expr    { $$ = make_node(SUB, $1, $3, 0); }
    | expr '*' expr    { $$ = make_node(MUL, $1, $3, 0); }
    | expr '/' expr    { $$ = make_node(DIV, $1, $3, 0); }
    | '(' expr ')'     { $$ = $2; }
;

%%

Node* make_node(int type, Node* left, Node* right, int value) {
    Node* n = malloc(sizeof(Node));
    n->type = type;
    n->left = left;
    n->right = right;
    n->value = value;
    return n;
}

Node* differentiate(Node* e) {
    switch (e->type) {

case NODE_TAN:
		return make_node(MUL,
			 make_node(DIV,
				   make_node(NUM, NULL, NULL, 1),
				   make_node(MUL,
					     make_node(NODE_COS, e->left, NULL, 0),
					     make_node(NODE_COS, e->left, NULL, 0),
					     0),
				   0),
			 differentiate(e->left),
			 0);

case NODE_EXP:
		return make_node(MUL,
			 make_node(NODE_EXP, e->left, NULL, 0),
			 differentiate(e->left),
			 0);

case NODE_LN:
                return make_node(DIV,
                         differentiate(e->left),
			 e->left,
                         0);

case NUM: return make_node(NUM, NULL, NULL, 0);
	case VAR: return make_node(NUM, NULL, NULL, 1);
	case ADD: return make_node(ADD, differentiate(e->left) , differentiate(e->right) , 0);
	case SUB: return make_node(SUB, differentiate(e->left), differentiate(e->right), 0);
	case MUL:
	    return make_node(ADD,
		make_node(MUL, differentiate(e->left), e->right, 0),
		make_node(MUL, e->left, differentiate(e->right), 0),
		0
	    );
	case DIV:
	    return make_node(DIV,
		make_node(SUB,
		    make_node(MUL, differentiate(e->left), e->right, 0),
		    make_node(MUL, e->left, differentiate(e->right), 0),
		    0
		),
		make_node(MUL, e->right, e->right, 0),
		0
	    );
	case NODE_SIN:
	    return make_node(MUL,
		differentiate(e->left),
		make_node(NODE_COS, e->left, NULL, 0),
		0
	    );
	case NODE_COS:
	    return make_node(MUL,
		make_node(NUM, NULL, NULL, -1),
		make_node(MUL,
		    simplify(differentiate(e->left)),
		    make_node(NODE_SIN, e->left, NULL, 0),
		    0
		),
		0
	    );
	default: return NULL;
    }
}


Node* simplify(Node* e) {
    if (!e) return NULL;

    if( !e->left )
	    e->left = simplify(e->left);

    if( !e->right )
	    e->right = simplify(e->right);

// Combine (x * x) + (x * (2 * x)) => 3 * x * x
    if (e->type == ADD &&
	!e->left && !e->right &&
	e->left->type == MUL && e->right->type == MUL) {

	Node *a1 = e->left->left, *a2 = e->left->right;
	Node *b1 = e->right->left, *b2 = e->right->right;

if (a1 && a2 && b1 && b2 &&
	    a1->type == VAR && a2->type == VAR &&
	    b1->type == VAR &&
	    b2->type == MUL && b2->left && b2->right &&
	    b2->left->type == NUM && b2->right->type == VAR) {

int coef = b2->left->value + 1;
	    Node* count = make_node(NUM, NULL, NULL, coef);
	    Node* var1 = make_node(VAR, NULL, NULL, 0);
	    Node* var2 = make_node(VAR, NULL, NULL, 0);
	    Node* inner = make_node(MUL, var1, var2, 0);
	    Node* res = make_node(MUL, count, inner, 0);
	    free_expr(e);
	    return simplify(res);
	}
    }




// Advanced combine: (a * x * x * x) + (b * x * x * x) => (a + b) * x * x * x
    if (e->type == ADD &&
	e->left && e->left->type == MUL &&
	e->right && e->right->type == MUL) {

Node *l1 = e->left, *r1 = e->right;

// Flatten helper
	Node* flatten_mul(Node* n, int* coef, Node** term) {
	    if (!n) return NULL;
	    if (n->type == NUM) {
		*coef *= n->value;
		return NULL;
	    } else if (n->type == VAR) {
		*term = make_node(VAR, NULL, NULL, 0);
		return NULL;
	    } else if (n->type == MUL) {
		flatten_mul(n->left, coef, term);
		flatten_mul(n->right, coef, term);
	    } else {
		*term = simplify(n);
	    }
	    return NULL;
	}

int coefL = 1, coefR = 1;
	Node *tL = NULL, *tR = NULL;
	flatten_mul(l1, &coefL, &tL);
	flatten_mul(r1, &coefR, &tR);

if (tL && tR && tL->type == MUL && tR->type == MUL &&
	    strcmp(print_to_string(tL), print_to_string(tR)) == 0) {
	    int total = coefL + coefR;
	    Node* const_node = make_node(NUM, NULL, NULL, total);
	    Node* new_mul = make_node(MUL, const_node, tL, 0);
	    free_expr(tR);
	    free_expr(e);
	    return simplify(new_mul);
	}
    }


// Identity simplifications

// Fold nested constant multiplications: 4 * (3 * e) => 12 * e
    if (e->type == MUL &&
	e->left && e->right &&
	e->left->type == NUM &&
	e->right->type == MUL &&
	e->right->left && e->right->left->type == NUM) {

int folded = e->left->value * e->right->left->value;
	Node* folded_const = make_node(NUM, NULL, NULL, folded);
	Node* new_mul = make_node(MUL, folded_const, e->right->right, 0);
	free(e->right->left);
	free(e->right);
	free(e->left);
	free(e);
	return simplify(new_mul);
    }

// 1 * e => e
    if (e->type == MUL) {
	if (e->left && e->left->type == NUM && e->left->value == 1) {
	    Node* res = e->right;
	    free(e->left);
	    free(e);
	    return simplify(res);
	}
	if (e->right && e->right->type == NUM && e->right->value == 1) {
	    Node* res = e->left;
	    free(e->right);
	    free(e);
	    return simplify(res);
	}
    }

// 0 * e => 0
    if (e->type == MUL) {
	if ((e->left && e->left->type == NUM && e->left->value == 0) ||
	    (e->right && e->right->type == NUM && e->right->value == 0)) {
	    free_expr(e);
	    return make_node(NUM, NULL, NULL, 0);
	}
    }

// e + 0 or 0 + e => e
    if (e->type == ADD) {
	if (e->left && e->left->type == NUM && e->left->value == 0) {
	    Node* res = e->right;
	    free(e->left);
	    free(e);
	    return simplify(res);
	}
	if (e->right && e->right->type == NUM && e->right->value == 0) {
	    Node* res = e->left;
	    free(e->right);
	    free(e);
	    return simplify(res);
	}
    }

// e - 0 => e
    if (e->type == SUB && e->right && e->right->type == NUM && e->right->value == 0) {
	Node* res = e->left;
	free(e->right);
	free(e);
	return simplify(res);
    }

// e / 1 => e
    if (e->type == DIV && e->right && e->right->type == NUM && e->right->value == 1) {
	Node* res = e->left;
	free(e->right);
	free(e);
	return simplify(res);
    }


// Simplify x + x => 2 * x
    if (e->type == ADD &&
	e->left && e->right &&
	e->left->type == VAR &&
	e->right->type == VAR) {
	Node* coeff = make_node(NUM, NULL, NULL, 2);
	Node* var = make_node(VAR, NULL, NULL, 0);
	Node* res = make_node(MUL, coeff, var, 0);
	free_expr(e);
	return simplify(res);
    }

// Combine (x * x) + (2 * x * x) => 3 * x * x
    if (e->type == ADD &&
	e->left && e->right &&
	e->left->type == MUL && e->right->type == MUL) {

Node *a1 = e->left->left, *a2 = e->left->right;
	Node *b1 = e->right->left, *b2 = e->right->right;

// Handle pattern: (x * x) + (NUM * (x * x))
	if (a1 && a2 && b1 && b2 &&
	    a1->type == VAR && a2->type == VAR &&
	    b1->type == NUM &&
	    b2->type == MUL && b2->left && b2->right &&
	    b2->left->type == VAR && b2->right->type == VAR) {

int coef = b1->value + 1;
	    Node* count = make_node(NUM, NULL, NULL, coef);
	    Node* var1 = make_node(VAR, NULL, NULL, 0);
	    Node* var2 = make_node(VAR, NULL, NULL, 0);
	    Node* inner = make_node(MUL, var1, var2, 0);
	    Node* res = make_node(MUL, count, inner, 0);
	    free_expr(e);
	    return simplify(res);
	}

// Match: (x * x) + (x * (x + x)) => 3 * x * x
	if (a1 && a2 && b1 && b2 &&
	    a1->type == VAR && a2->type == VAR &&
	    b1->type == VAR &&
	    b2->type == ADD &&
	    b2->left && b2->right &&
	    b2->left->type == VAR && b2->right->type == VAR) {

Node* count = make_node(NUM, NULL, NULL, 3);
	    Node* var1 = make_node(VAR, NULL, NULL, 0);
	    Node* var2 = make_node(VAR, NULL, NULL, 0);
	    Node* inner = make_node(MUL, var1, var2, 0);
	    Node* res = make_node(MUL, count, inner, 0);
	    free_expr(e);
	    return simplify(res);
	}
    }
    // Combine (a * x * x) + (b * x * x) => (a + b) * x * x
    if (e->type == ADD &&
	e->left && e->right &&
	e->left->type == MUL && e->right->type == MUL) {

Node *a1 = e->left->left, *a2 = e->left->right;
	Node *b1 = e->right->left, *b2 = e->right->right;

// Normalize if NUM is on the right
	if (a1->type == NUM) { Node* tmp = a1; a1 = a2; a2 = tmp; }
	if (b1->type == NUM) { Node* tmp = b1; b1 = b2; b2 = tmp; }

if (a2->type == NUM && b2->type == NUM &&
	    a1->type == MUL && b1->type == MUL &&
	    a1->left->type == VAR && a1->right->type == VAR &&
	    b1->left->type == VAR && b1->right->type == VAR) {

int coef = a2->value + b2->value;
	    Node* folded = make_node(NUM, NULL, NULL, coef);
	    Node* var1 = make_node(VAR, NULL, NULL, 0);
	    Node* var2 = make_node(VAR, NULL, NULL, 0);
	    Node* inner = make_node(MUL, var1, var2, 0);
	    Node* res = make_node(MUL, folded, inner, 0);
	    free_expr(e);
	    return simplify(res);
	}
    }

return e;
}


void print_expr(Node* e) {
    if (!e) return;
    switch (e->type) {
	case NUM: printf("%d", e->value); break;
	case VAR: printf("x"); break;
	case ADD: printf("("); print_expr(e->left); printf(" + "); print_expr(e->right); printf(")"); break;
	case SUB: printf("("); print_expr(e->left); printf(" - "); print_expr(e->right); printf(")"); break;
	case MUL: printf("("); print_expr(e->left); printf(" * "); print_expr(e->right); printf(")"); break;
	case DIV: printf("("); print_expr(e->left); printf(" / "); print_expr(e->right); printf(")"); break;
	case NODE_SIN: printf("sin("); print_expr(e->left); printf(")"); break;
	case NODE_COS: printf("cos("); print_expr(e->left); printf(")"); break;
	case NODE_TAN: printf("tan("); print_expr(e->left); printf(")"); break;
	case NODE_LN: printf("ln("); print_expr(e->left); printf(")"); break;
    }
}

void free_expr(Node* e) {
    if (!e) return;

if( !e->left ) 
	    free_expr(e->left);
    if ( !e->right )
	    free_expr(e->right);
/*
    free(e);
*/
}

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

char* print_to_string_rec(Node* e, char* buf) {
    if (!e) return buf;

switch (e->type) {
	case NUM:
	    buf += sprintf(buf, "%d", e->value);
	    break;
	case VAR:
	    buf += sprintf(buf, "x");
	    break;
	case ADD:
	    *buf++ = '(';
	    buf = print_to_string_rec(e->left, buf);
	    buf += sprintf(buf, "+");
	    buf = print_to_string_rec(e->right, buf);
	    *buf++ = ')';
	    break;
	case SUB:
	    *buf++ = '(';
	    buf = print_to_string_rec(e->left, buf);
	    buf += sprintf(buf, "-");
	    buf = print_to_string_rec(e->right, buf);
	    *buf++ = ')';
	    break;
	case MUL:
	    *buf++ = '(';
	    buf = print_to_string_rec(e->left, buf);
	    buf += sprintf(buf, "*");
	    buf = print_to_string_rec(e->right, buf);
	    *buf++ = ')';
	    break;
	case DIV:
	    *buf++ = '(';
	    buf = print_to_string_rec(e->left, buf);
	    buf += sprintf(buf, "/");
	    buf = print_to_string_rec(e->right, buf);
	    *buf++ = ')';
	    break;
	case NODE_SIN:
	    buf += sprintf(buf, "sin(");
	    buf = print_to_string_rec(e->left, buf);
	    *buf++ = ')';
	    break;
	case NODE_COS:
	    buf += sprintf(buf, "cos(");
	    buf = print_to_string_rec(e->left, buf);
	    *buf++ = ')';
	    break;
	case NODE_EXP:
	    buf += sprintf(buf, "exp(");
	    buf = print_to_string_rec(e->left, buf);
	    *buf++ = ')';
	    break;
	case NODE_TAN:
	    buf += sprintf(buf, "tan(");
	    buf = print_to_string_rec(e->left, buf);
	    *buf++ = ')';
	    break;
    }
    *buf = '\0';
    return buf;
}

char* print_to_string(Node* e) {
    static char buffer[1024];
    buffer[0] = '\0';
    print_to_string_rec(e, buffer);
    return buffer;
}


void main(void) {

#ifndef __RELEASE 
const char* test_cases[] = {
	"sin(x)", "cos(x)", "tan(x)", 
	"(4*x*x*x)+(8*x*x*x)*x",
	"tan(exp(x))",
	"tan(exp(x+exp(x)))",
	"cos(2*x)",
	"ln( cos(x))"
    };

size_t num_cases = sizeof(test_cases) / sizeof(test_cases[0]);

printf(" running %zu tests\n", num_cases);

for (size_t i = 0; i < num_cases; ++i) {
	yy_scan_string(test_cases[i]);
	yyparse();
    }
#else
 yyparse();
#endif
}
