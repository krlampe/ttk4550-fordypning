#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ast.h"

ast_node *new_ast_node(char nodetype, ast_node *left, ast_node *right) {
  ast_node *node = malloc(sizeof *node);
  if(node == NULL) {
		fprintf(stderr, "Failed to allocate memory\n");
		exit(1);
	}

	node->nodetype = nodetype;
	node->left = left;
	node->right = right;
	return node;
}

ast_node *new_ast_symbol(char nodetype, char *name) {
  ast_symbol *node = malloc(sizeof *node);
  if(node == NULL) {
		fprintf(stderr, "Failed to allocate memory\n");
		exit(1);
	}

	node->nodetype = nodetype;
  node->name = strdup(name);
  return (ast_node *)node;
}

ast_node *new_ast_numeral(char *name) {
  ast_number *node = malloc(sizeof *node);
  if(node == NULL) {
		fprintf(stderr, "Failed to allocate memory\n");
		exit(1);
	}

	node->nodetype = 'N';
  node->numeral = strdup(name);
  return (ast_node *)node;
}

char *ast_get_LHS(ast_node *root) {
	return ((ast_symbol *)root->left)->name;
}

void ast_traverse_RHS(ast_node *root, FILE *out) {
	switch(root->nodetype) {
	case '=':
		/* Just traverse to the right */
		ast_traverse(root->right, out);
		break;
	case '+':
	case '-':
		fprintf(out, "%s","(");
		ast_traverse(root->left, out);
		fprintf(out, " %c ", root->nodetype); // spaces around
		ast_traverse(root->right, out);
		fprintf(out, "%s",")");
		break;
	case '*':
	case '/':
	case '^':
		fprintf(out, "%s","(");
		ast_traverse(root->left, out);
		fprintf(out, "%c", root->nodetype); // without spaces around
		ast_traverse(root->right, out);
		fprintf(out, "%s",")");
		break;
	case 'M':
		fprintf(out, "%s","-");
		ast_traverse(root->left, out);
		break;
	case 'A':
		fprintf(out, "%s","|");
		ast_traverse(root->left, out);
		fprintf(out, "%s","|");
		break;
	case 'C':
		ast_traverse(root->left, out);
		fprintf(out, "%s","(");
		ast_traverse(root->right, out);
		fprintf(out, "%s",")");
		break;

	/* Leaf nodes */
	case 'F':
	case 'T':
	case 'B':
		fprintf(out, "%s", ((ast_symbol *)root)->name);
		break;
	case 'N':
		fprintf(out, "%s", ((ast_number *)root)->numeral);
		break;

	case 'D':
	default:
		fprintf(stderr, "Bad nodetype %c\n", root->nodetype);
		break;
	}
}

void ast_traverse(ast_node *root, FILE *out) {
	switch(root->nodetype) {
	case '=':
		ast_traverse(root->left, out);
		fprintf(out, " %c ", root->nodetype);
		ast_traverse(root->right, out);
		break;
	case '+':
	case '-':
		fprintf(out, "%s","(");
		ast_traverse(root->left, out);
		fprintf(out, " %c ", root->nodetype); // spaces around
		ast_traverse(root->right, out);
		fprintf(out, "%s",")");
		break;
	case '*':
	case '/':
	case '^':
		fprintf(out, "%s","(");
		ast_traverse(root->left, out);
		fprintf(out, "%c", root->nodetype); // without spaces around
		ast_traverse(root->right, out);
		fprintf(out, "%s",")");
		break;
	case 'M':
		fprintf(out, "%s","-");
		ast_traverse(root->left, out);
		break;
	case 'A':
		fprintf(out, "%s","|");
		ast_traverse(root->left, out);
		fprintf(out, "%s","|");
		break;
	case 'C':
		ast_traverse(root->left, out);
		fprintf(out, "%s","(");
		ast_traverse(root->right, out);
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
