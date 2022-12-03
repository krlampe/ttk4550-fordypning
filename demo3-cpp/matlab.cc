#include <stdlib.h>
#include <stdio.h>
#include "matlab.hh"
#include "ast.hh"

/**
 * Modify the AST of an equation so that the names of nodes
 * are matlab compatible. Variable names are converted to
 * y(i), where i is the variable's index in the matlab matrix y,
 * which is equal to its index in the symbol table plus 1.
 * (Matlab has 1-based indexing.)
*/
static void make_matlab_AST(ast_node *root);

/**
 * After make_matlab_AST() has processed the AST, print
 * the right-hand side in matlab format.
*/
static void print_matlab_AST(ast_node *root, FILE *out);

void matlab::generate(FILE *out) {
  int nr_of_equations = symbol_table_get_length();

  /* Heading */
  fputs("%% Simulator generated by sim-gen.\n% ODE:\n", out);
  for (int i = 0; i < nr_of_equations; i++) {
    fputs("% ", out);
    ast_print(symbol_table[i].ast_root, out);
    fputs("\n", out);
  }

  /* Modify all the ASTs */
  for (int i = 0; i < nr_of_equations; i++) {
    make_matlab_AST(symbol_table[i].ast_root);
  }

  /*
  Print matlab formatet equations:
      dydt = @(t, y) [<equations>];
  */
  fputs("\ndydt = @(t, y) [", out);
  for (int i = 0; i < nr_of_equations; i++) {
    print_matlab_AST(symbol_table[i].ast_root, out);
    fputs(";", out);
  }
  fputs("];\n", out);

  /* Initial values */
  fputs("y_0 = [0", out);
  for (int i = 1; i < nr_of_equations; i++) {
    fputs(", 0", out);
  }
  fputs("];\n", out);

  /* Integrator */
  fputs("[t,y] = ode45(dydt, [0 20], y_0);\n", out);

  /* Plotting */
  fputs(
    "figure; clf;\n"
    "hold on;\n",
    out
  );
  for (int i = 0; i < nr_of_equations; i++) {
    fprintf(out, "plot(t,y(:,%d),'-o');\n", i+1);
  }
  fputs(
    "hold off;\n"
    "grid on;\n"
    "xlabel('Time t');\n"
    "ylabel('Solution');\n",
    out
  );
  fprintf(out, "legend('%s'", symbol_table[0].name);
  for (int i = 1; i < nr_of_equations; i++) {
    fprintf(out, ", '%s'", symbol_table[i].name);
  }
  fputs(");\n", out);
}

static void make_matlab_AST(ast_node *root) {
  switch((root->nodetype)) {
  /* Convert variable names to y(i), using 1-based indexing */
	case 'F': {
    symbol *sym = symbol_table_lookup(((ast_symbol *)root)->name);
    if (sym) {
      ((ast_symbol *)root)->name = (char*)realloc(((ast_symbol *)root)->name, 7);
      sprintf(((ast_symbol *)root)->name, "y(%d)",sym->index+1);
    } else {
      fprintf(stderr, "Symbol %s not defined\n", ((ast_symbol *)root)->name);
    }
  } break;

	/* Binary nodes */
	case '=':	case '+':	case '-':	case '*':	case '/':	case '^':
	case 'C':
		make_matlab_AST(root->right);
	/* Unary nodes */
	case 'M':
	case 'A':
		make_matlab_AST(root->left);
  default:
    break;
  }
}

static void print_matlab_AST(ast_node *root, FILE *out) {
	switch(root->nodetype) {
	case '=':
		print_matlab_AST(root->right, out);
		break;
	case '+':
	case '-':
	case '*':
	case '/':
	case '^':
		fprintf(out, "%s","(");
		print_matlab_AST(root->left, out);
		fprintf(out, "%c", root->nodetype);
		print_matlab_AST(root->right, out);
		fprintf(out, "%s",")");
		break;
	case 'M':
		fprintf(out, "%s","-");
		print_matlab_AST(root->left, out);
		break;
	case 'A':
		fprintf(out, "%s","abs(");
		print_matlab_AST(root->left, out);
		fprintf(out, "%s",")");
		break;
	case 'C':
		print_matlab_AST(root->left, out);
		fprintf(out, "%s","(");
		print_matlab_AST(root->right, out);
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
		fprintf(stderr, "matlab_generate: Bad nodetype %c\n", root->nodetype);
		break;
	}
}