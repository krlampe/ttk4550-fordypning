#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ast.hh"

/* From parser */
void yyerror(const char *msg);

#define SYMBOL_TABLE_MAX_LENGTH 1024
symbol symbol_table[SYMBOL_TABLE_MAX_LENGTH];
static int symbol_table_length = 0;

ast_node *new_ast_node(char nodetype, ast_node *left, ast_node *right) {
  ast_node *node = (ast_node*)malloc(sizeof *node);
  if(!node) {
		perror("abort");
		exit(1);
	}

	node->nodetype = nodetype;
	node->left = left;
	node->right = right;
	return node;
}

ast_node *new_ast_symbol(char nodetype, char *name) {
  ast_symbol *node = (ast_symbol*)malloc(sizeof *node);
  if(!node) {
		perror("abort");
		exit(1);
	}

	node->nodetype = nodetype;
  node->name = strdup(name);
  return (ast_node *)node;
}

ast_node *new_ast_numeral(char *name) {
  ast_number *node = (ast_number*)malloc(sizeof *node);
  if(!node) {
		perror("abort");
		exit(1);
	}

	node->nodetype = 'N';
  node->numeral = strdup(name);
  return (ast_node *)node;
}

void ast_print(ast_node *root, FILE *out) {
	switch(root->nodetype) {
	case '=':
		ast_print(root->left, out);
		fprintf(out, " %c ", root->nodetype);
		ast_print(root->right, out);
		break;
	case '+':
	case '-':
		fprintf(out, "%s","(");
		ast_print(root->left, out);
		fprintf(out, " %c ", root->nodetype); // spaces around
		ast_print(root->right, out);
		fprintf(out, "%s",")");
		break;
	case '*':
	case '/':
	case '^':
		fprintf(out, "%s","(");
		ast_print(root->left, out);
		fprintf(out, "%c", root->nodetype); // without spaces around
		ast_print(root->right, out);
		fprintf(out, "%s",")");
		break;
	case 'M':
		fprintf(out, "%s","-");
		ast_print(root->left, out);
		break;
	case 'A':
		fprintf(out, "%s","|");
		ast_print(root->left, out);
		fprintf(out, "%s","|");
		break;
	case 'C':
		ast_print(root->left, out);
		fprintf(out, "%s","(");
		ast_print(root->right, out);
		fprintf(out, "%s",")");
		break;

	/* Leaf nodes */
	case 'F':
	case 'T':
	case 'B':
		fprintf(out, "%s", ((ast_symbol *)root)->name);
		break;
	case 'D':
		fprintf(out, "(d/dt)%s", ((ast_symbol *)root)->name);
		break;
	case 'N':
		fprintf(out, "%s", ((ast_number *)root)->numeral);
		break;
	default:
		fprintf(stderr, "Bad nodetype %c\n", root->nodetype);
		break;
	}
}

void ast_free(ast_node *root) {
	switch(root->nodetype) {
	/* Binary nodes */
	case '=':	case '+':	case '-':	case '*':	case '/':	case '^':
	case 'C':
		ast_free(root->right);
	/* Unary nodes */
	case 'M':
	case 'A':
		ast_free(root->left);
		break;

	/* Leaf nodes */
	case 'F':
	case 'T':
	case 'B':
	case 'D':
		free(((ast_symbol *)root)->name);
		break;
	case 'N':
		free(((ast_number *)root)->numeral);
		break;
	default:
		fprintf(stderr, "Free bad nodetype %c\n", root->nodetype);
		break;
	}
	free(root);
}


void symbol_table_add(char *name, ast_node *ast_root) {
  for (int i = 0; i < SYMBOL_TABLE_MAX_LENGTH; i++) {
    symbol *sym = &symbol_table[i];
    
    if (sym->name && !strcmp(name, sym->name)) {
  		yyerror("symbol defined twice");
  		exit(1);
		}

    if (!sym->name) { // New entry
			sym->index = i;
      sym->name = strdup(name);
      sym->ast_root = ast_root;
			++symbol_table_length;
			return;
    }
  }
  yyerror("symbol table overflow");
}

int symbol_table_get_length(void) {
	return symbol_table_length;
}

symbol *symbol_table_lookup(char *name) {
  for (int i = 0; i < symbol_table_length; i++) {
    symbol *sym = &symbol_table[i];
    if (sym->name && !strcmp(name, sym->name)) return sym;
  }
	return NULL;
}

void symbol_table_free(void) {
  for (int i = 0; i < symbol_table_length; i++) {
		symbol *sym = &symbol_table[i];
    if (!sym->name) return;
    free(sym->name);
    if (sym->ast_root->nodetype) ast_free(sym->ast_root);
  }
}

void symbol_table_print(void) {
  for (int i = 0; i < SYMBOL_TABLE_MAX_LENGTH; i++) {
		symbol *sym = &symbol_table[i];
    if (!sym->name) return;
    puts(sym->name);
  }
}
