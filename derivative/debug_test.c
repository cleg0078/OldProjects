#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Copy relevant structs and functions from expr.y

typedef struct Node {
    enum { NUM, VAR, ADD, SUB, MUL, DIV, NODE_SIN, NODE_COS, NODE_TAN, NODE_EXP, NODE_LN, NODE_LOG } type;
    struct Node *left, *right;
    int value;
} Node;

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
        case VAR: return make_node(NUM, NULL, NULL, 1);
        case NUM: return make_node(NUM, NULL, NULL, 0);
        default: return NULL;
    }
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
    }
}

int main() {
    // Build cos(x)
    Node* cos_x = make_node(NODE_COS, make_node(VAR, NULL, NULL, 0), NULL, 0);
    printf("Input: ");
    print_expr(cos_x);
    printf("\n\n");
    
    // Differentiate
    Node* deriv = differentiate(cos_x);
    printf("After differentiation: ");
    print_expr(deriv);
    printf("\n\n");
    
    // The expected result should be: MUL(NUM(-1), MUL(1, SIN(x)), 0)
    // which simplifies to: -1 * (1 * sin(x)) = -sin(x)
    
    return 0;
}
