%code requires {
#ifndef NODE_DEFINED
#define NODE_DEFINED
	typedef struct Node {
		enum { NUM, VAR, ADD, SUB, MUL, DIV, NODE_SIN, NODE_COS, NODE_TAN, NODE_EXP, NODE_LN, NODE_LOG, NODE_POW } type;
		struct Node *left, *right;
		int value;
	} Node;
#endif
}

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef __RELEASE 
extern void yy_scan_string(const char *);
#endif

#ifndef NODE_DEFINED
#define NODE_DEFINED
	typedef struct Node {
		enum { NUM, VAR, ADD, SUB, MUL, DIV, NODE_SIN, NODE_COS, NODE_TAN, NODE_EXP, NODE_LN, NODE_LOG, NODE_POW } type;
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
%token SIN COS TAN EXP LN LOG POW
%type <node> expr func


%left '+' '-'
%right '^'
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
| LN '(' expr ')'     { $$ = make_node(NODE_LN, $3, NULL, 0);  }
| LOG '(' expr ')'    { $$ = make_node(NODE_LOG, $3, NULL, 0);  };

expr:
NUMBER           { $$ = make_node(NUM, NULL, NULL, $1); }
| VARIABLE         { $$ = make_node(VAR, NULL, NULL, 0); }
| func             { $$ = $1; }
| expr '+' expr    { $$ = make_node(ADD, $1, $3, 0); }
| expr '-' expr    { $$ = make_node(SUB, $1, $3, 0); }
| expr '*' expr    { $$ = make_node(MUL, $1, $3, 0); }
| expr '/' expr    { $$ = make_node(DIV, $1, $3, 0); }
| expr POW expr    { $$ = make_node(NODE_POW, $1, $3, 0); }
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
		case NODE_LOG:
			return make_node(DIV,
					differentiate(e->left),
					e->left,
					0);
		case NODE_POW:
			// d/dx(u^v) = v*u^(v-1)*u' + u^v*ln(u)*v' (general power rule)
			// For v = constant: d/dx(u^n) = n*u^(n-1)*u'
			// For u = x, v = n (constant): d/dx(x^n) = n*x^(n-1)
			if (e->right && e->right->type == NUM) {
				// d/dx(u^n) = n*u^(n-1)*u'
				int n = e->right->value;
				return make_node(MUL,
						make_node(NUM, NULL, NULL, n),
						make_node(MUL,
								make_node(NODE_POW, e->left, make_node(NUM, NULL, NULL, n-1), 0),
								differentiate(e->left),
								0),
						0);
			} else {
				// General case: d/dx(u^v) = u^v * (v*u^(v-1)*u' + ln(u)*v')
				return make_node(MUL,
						make_node(NODE_POW, e->left, e->right, 0),
						make_node(ADD,
								make_node(MUL,
										e->right,
										make_node(MUL,
												make_node(NODE_POW, e->left, make_node(NUM, NULL, NULL, 1), 0),
												differentiate(e->left),
												0),
										0),
								make_node(MUL,
										make_node(NODE_LN, e->left, NULL, 0),
										differentiate(e->right),
										0),
								0),
						0);
			}


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
						  differentiate(e->left),
						  make_node(NODE_SIN, e->left, NULL, 0),
						  0
						  ),
					  0
					  );
		default: return NULL;
	}
}

// Forward declarations
Node* distribute(Node* e);
Node* evaluate_constant(Node* e);
int nodes_equal(Node* a, Node* b);
void term_from_product(Node* e, int* coef, int* x_power, Node** trans);
typedef struct Term {
	int coef;
	int x_power;
	Node* trans;
	struct Term* next;
} Term;
Term* flatten_to_terms(Node* e);
Term* negate_terms(Term* t);
Term* concat_terms(Term* a, Term* b);
void free_terms(Term* t);
Term* collect_terms(Term* t);
Node* terms_to_expr(Term* t);
Node* deep_copy_node(Node* e);
Node* simplify_arithmetic(Node* e);

Node* simplify(Node* e) {
	if (!e) return NULL;

	// Deep copy to avoid destroying the input tree
	e = deep_copy_node(e);

	// First, distribute and evaluate constants
	e = distribute(e);
	e = evaluate_constant(e);

	// Flatten to terms, collect, rebuild
	Term* terms = flatten_to_terms(e);
	free_expr(e);

	Term* collected = collect_terms(terms);
	terms = NULL;
	free_terms(terms);

	Node* result = terms_to_expr(collected);
	free_terms(collected);

	return result;
}

// Simplify arithmetic expressions: 0*x=0, 1*x=x, 0+x=x
Node* simplify_arithmetic(Node* e) {
    if (!e) return NULL;
    
    e->left = simplify_arithmetic(e->left);
    e->right = simplify_arithmetic(e->right);
    
    // 0 * x = 0
    if (e->type == MUL && e->left && e->left->type == NUM && e->left->value == 0) {
        Node* res = make_node(NUM, NULL, NULL, 0);
        free(e);
        return res;
    }
    
    // x * 0 = 0
    if (e->type == MUL && e->right && e->right->type == NUM && e->right->value == 0) {
        Node* res = make_node(NUM, NULL, NULL, 0);
        free(e);
        return res;
    }
    
    // 1 * x = x
    if (e->type == MUL && e->left && e->left->type == NUM && e->left->value == 1) {
        Node* res = e->right;
        e->right = NULL;
        free(e);
        return res;
    }
    
    // x * 1 = x
    if (e->type == MUL && e->right && e->right->type == NUM && e->right->value == 1) {
        Node* res = e->left;
        e->left = NULL;
        free(e);
        return res;
    }
    
    // 0 + x = x
    if (e->type == ADD && e->left && e->left->type == NUM && e->left->value == 0) {
        Node* res = e->right;
        e->right = NULL;
        free(e);
        return res;
    }
    
    // x + 0 = x
    if (e->type == ADD && e->right && e->right->type == NUM && e->right->value == 0) {
        Node* res = e->left;
        e->left = NULL;
        free(e);
        return res;
    }
    
    // x - 0 = x
    if (e->type == SUB && e->right && e->right->type == NUM && e->right->value == 0) {
        Node* res = e->left;
        e->left = NULL;
        free(e);
        return res;
    }
    
    // 0 - x = -x
    if (e->type == SUB && e->left && e->left->type == NUM && e->left->value == 0) {
        Node* neg = make_node(MUL, make_node(NUM, NULL, NULL, -1), e->right, 0);
        e->right = NULL;
        free(e);
        return simplify_arithmetic(neg);
    }
    
    // x / 1 = x
    if (e->type == DIV && e->right && e->right->type == NUM && e->right->value == 1) {
        Node* res = e->left;
        e->left = NULL;
        free(e);
        return res;
    }
    
    return e;
}


Node* deep_copy_node(Node* e) {
	if (!e) return NULL;
	Node* n = make_node(e->type, NULL, NULL, e->value);
	n->left = deep_copy_node(e->left);
	n->right = deep_copy_node(e->right);
	return n;
}

// Distribute multiplication over addition: (a+b)*c => a*c + b*c
Node* distribute(Node* e) {
	if (!e) return NULL;
	e->left = distribute(e->left);
	e->right = distribute(e->right);

	if (e->type == MUL && e->left && e->right && e->left->type == ADD) {
		Node* a = e->left->left;
		Node* b = e->left->right;
		Node* c = e->right;
		Node* new_add = make_node(ADD, make_node(MUL, a, c, 0), make_node(MUL, b, c, 0), 0);
		free(e->left);
		free(e);
		return distribute(new_add);
	}

	if (e->type == MUL && e->left && e->right && e->right->type == ADD) {
		Node* a = e->left;
		Node* b = e->right->left;
		Node* c = e->right->right;
		Node* new_add = make_node(ADD, make_node(MUL, a, b, 0), make_node(MUL, a, c, 0), 0);
		free(e->right);
		free(e);
		return distribute(new_add);
	}

	return e;
}

// Evaluate constant subexpressions: sin(0)=0, cos(0)=1, exp(0)=1, ln(1)=0
Node* evaluate_constant(Node* e) {
	if (!e) return NULL;
	e->left = evaluate_constant(e->left);
	e->right = evaluate_constant(e->right);

	if (e->type == NODE_SIN && e->left && e->left->type == NUM && e->left->value == 0) {
		Node* res = make_node(NUM, NULL, NULL, 0);
		free(e);
		return res;
	}
	if (e->type == NODE_COS && e->left && e->left->type == NUM && e->left->value == 0) {
		Node* res = make_node(NUM, NULL, NULL, 1);
		free(e);
		return res;
	}
	if (e->type == NODE_EXP && e->left && e->left->type == NUM && e->left->value == 0) {
		Node* res = make_node(NUM, NULL, NULL, 1);
		free(e);
		return res;
	}
	if ((e->type == NODE_LN || e->type == NODE_LOG) && e->left && e->left->type == NUM && e->left->value == 1) {
		Node* res = make_node(NUM, NULL, NULL, 0);
		free(e);
		return res;
	}

	return e;
}

// Check if two nodes are structurally equal
int nodes_equal(Node* a, Node* b) {
	if (!a && !b) return 1;
	if (!a || !b) return 0;
	if (a->type != b->type) return 0;
	if (a->type == NUM) return a->value == b->value;
	if (a->type == VAR) return 1;
	if (nodes_equal(a->left, b->left) && nodes_equal(a->right, b->right)) return 1;
	return 0;
}

// Helper function to process a product chain without resetting
void term_from_product_inner(Node* e, int* coef, int* x_power, Node** trans) {
	while (e) {
		if (e->type == NUM) {
			*coef *= e->value;
			e = NULL;
		} else if (e->type == VAR) {
			(*x_power)++;
			e = NULL;
		} else if (e->type == MUL) {
			term_from_product_inner(e->left, coef, x_power, trans);
			e = e->right;
		} else {
			if (*trans) {
				*trans = make_node(MUL, *trans, e, 0);
			} else {
				*trans = e;
			}
			e = NULL;
		}
	}
}

// Extract coefficient, x_power, and transcendental part from a product chain
void term_from_product(Node* e, int* coef, int* x_power, Node** trans) {
	*coef = 1;
	*x_power = 0;
	*trans = NULL;
	term_from_product_inner(e, coef, x_power, trans);
}

// Convert AST to linked list of terms
Term* flatten_to_terms(Node* e) {
	if (!e) return NULL;

	if (e->type == ADD) {
		return concat_terms(flatten_to_terms(e->left), flatten_to_terms(e->right));
	}

	if (e->type == SUB) {
		Term* right = flatten_to_terms(e->right);
		right = negate_terms(right);
		return concat_terms(flatten_to_terms(e->left), right);
	}

	// Single term (product or atomic)
	int coef = 1, x_power = 0;
	Node* trans = NULL;
	term_from_product(e, &coef, &x_power, &trans);

	Term* t = malloc(sizeof(Term));
	t->coef = coef;
	t->x_power = x_power;
	t->trans = trans;
	t->next = NULL;
	return t;
}

Term* negate_terms(Term* t) {
	if (!t) return NULL;
	t->coef = -t->coef;
	return t;
}

Term* concat_terms(Term* a, Term* b) {
	if (!a) return b;
	if (!b) return a;
	Term* t = a;
	while (t->next) t = t->next;
	t->next = b;
	return a;
}

void free_terms(Term* t) {
	while (t) {
		Term* next = t->next;
		if (t->trans) free_expr(t->trans);
		free(t);
		t = next;
	}
}

// Collect terms with matching (x_power, trans)
Term* collect_terms(Term* t) {
	if (!t) return NULL;

	Term* result = NULL;
	while (t) {
		Term* next = t->next;
		t->next = NULL;

		Term* match = result;
		Term* prev = NULL;
		while (match) {
			if (match->x_power == t->x_power && nodes_equal(match->trans, t->trans)) {
				match->coef += t->coef;
				t->trans = NULL;  // Detach to avoid double-free
				t->next = NULL;  // Prevent double-free
				if (prev) prev->next = match;
				else result = match;
				// Remove term if coefficient is 0
				if (match->coef == 0) {
					Term* to_remove = match;
					if (prev) prev->next = match->next;
					else result = match->next;
					free_terms(to_remove);
				}
				break;
			}
			prev = match;
			match = match->next;
		}

		if (!match) {
			t->next = result;
			result = t;
		}

		t = next;
	}

	// Remove any remaining terms with coefficient 0
	Term** pp = &result;
	while (*pp) {
		Term* curr = *pp;
		if (curr->coef == 0) {
			*pp = curr->next;
			curr->next = NULL;
			free_terms(curr);
		} else {
			pp = &curr->next;
		}
	}

	return result;
}

// Rebuild AST from collected terms
Node* terms_to_expr(Term* t) {
	if (!t) return make_node(NUM, NULL, NULL, 0);

	Node* result = NULL;
	while (t) {
		Node* term_expr = NULL;
		Node* trans_copy = t->trans ? deep_copy_node(t->trans) : NULL;

		if (t->trans && t->x_power > 0) {
			Node* vars = NULL;
			for (int i = 0; i < t->x_power; i++) {
				Node* v = make_node(VAR, NULL, NULL, 0);
				vars = vars ? make_node(MUL, vars, v, 0) : v;
			}
			term_expr = make_node(MUL, vars, trans_copy, 0);
		} else if (t->x_power > 0) {
			Node* vars = NULL;
			for (int i = 0; i < t->x_power; i++) {
				Node* v = make_node(VAR, NULL, NULL, 0);
				vars = vars ? make_node(MUL, vars, v, 0) : v;
			}
			term_expr = vars;
		} else if (t->trans) {
			term_expr = trans_copy;
		} else {
			term_expr = make_node(NUM, NULL, NULL, t->coef);
		}

		if (t->coef != 1) {
			term_expr = make_node(MUL, make_node(NUM, NULL, NULL, t->coef), term_expr, 0);
		}

		result = result ? make_node(ADD, result, term_expr, 0) : term_expr;
		t = t->next;
	}

	return result;
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
		case NODE_LOG: printf("log("); print_expr(e->left); printf(")"); break;
		case NODE_POW: print_expr(e->left); printf("^"); print_expr(e->right); break;
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
		printf("solving %s=", test_cases[i] );
		yy_scan_string(test_cases[i]);
		yyparse();
	}
#else
	yyparse();
#endif
}
