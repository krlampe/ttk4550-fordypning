/*
* Abstract syntax tree for ordinary differential equations.
*/

#ifndef AST_H
#define AST_H

/*
* Node types
* Binary operators: + - * / ^
* = - Equation
* M - Unary minus
* A - Absolute value
* C - Built-in function call
*
* Leaf node types
* F - Function symbol
* D - Derivative of a function
* T - Independent variable t
* B - Built-in function symbol
* N - Number
*/

/* Nodes in the abstract syntax tree */
typedef struct ast_node ast_node;
struct ast_node {
	char nodetype; /* Equation, operator, M, A or C */
	ast_node *left;
	ast_node *right;
};

typedef struct {
  char nodetype; /* type F, D, T, or B */
  char *name;
} ast_symbol;

typedef struct  {
  char nodetype; /* type N */
  char *numeral;
} ast_number;

ast_node *new_ast_node(char nodetype, ast_node *left, ast_node *right);
ast_node *new_ast_symbol(char nodetype, char *name);
ast_node *new_ast_numeral(char *name);
void ast_free(ast_node *root);

/* Traverse the AST, writing to an output stream */
void ast_traverse(ast_node *root, FILE *out);
void ast_traverse_RHS(ast_node *root, FILE *out);

/* Returns a pointer to the name on the LHS */
char *ast_get_LHS(ast_node *root);

#endif /* AST_H */