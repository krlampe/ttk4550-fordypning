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

void ast_traverse(ast_node *root) {
	switch(root->nodetype) {
	case '=':
		printf("Equation defining the derivative of %s:\n\t", ((ast_symbol *)root->left)->name);
		ast_traverse(root->left);
		printf(" %c ", root->nodetype);
		ast_traverse(root->right);
		break;
	case '+':
	case '-':
		printf("%s","(");
		ast_traverse(root->left);
		printf(" %c ", root->nodetype); /* Spaces around */
		ast_traverse(root->right);
		printf("%s",")");
		break;
	case '*':
	case '/':
	case '^':
		printf("%s","(");
		ast_traverse(root->left);
		printf("%c", root->nodetype); /* Without spaces around */
		ast_traverse(root->right);
		printf("%s",")");
		break;
	case 'M':
		printf("%s","-");
		ast_traverse(root->left);
		break;
	case 'A':
		printf("%s","|");
		ast_traverse(root->left);
		printf("%s","|");
		break;
	case 'C':
		ast_traverse(root->left);
		printf("%s","(");
		ast_traverse(root->right);
		printf("%s",")");
		break;

	/* Leaf nodes */
	case 'F':
	case 'T':
	case 'B':
		printf("%s", ((ast_symbol *)root)->name);
		break;
	case 'D':
		printf("(d/dt)%s", ((ast_symbol *)root)->name);
		break;
	case 'N':
		printf("%s", ((ast_number *)root)->numeral);
		break;
	default:
		printf("Bad nodetype %c\n", root->nodetype);
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
		printf("Free bad nodetype %c\n", root->nodetype);
		break;
	}
	free(root);
}
