/*
* Abstract syntax tree and symbol table for ordinary differential equations.
*/

#ifndef AST_H
#define AST_H

/*
* Node types of the Abstract syntax tree
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

/* Traverse an AST, writing to an output stream */
void ast_print(ast_node *root, FILE *out);

/* Symbol table */
typedef struct {
  int index;
  char *name;
  ast_node *ast_root;
} symbol;

extern symbol symbol_table[];

int symbol_table_get_length(void);
void symbol_table_add(char *name, ast_node *ast_root);
void symbol_table_free(void);
void symbol_table_print(void);

/* Return symbol pointer if it exists, NULL otherwise */
symbol *symbol_table_lookup(char *name);

#endif /* AST_H */