#include <stdio.h>
#include <string>
#include "ast.hh"
#include "strategy.hh"

/**
 * Print RHS of differential equations for latex code by visiting ASTs.
 */
class LatexPrinter : public AstVisitor {
public:
  LatexPrinter(FILE *outputfile) : out{outputfile} {}

  void visit(AstNumber *node);
  void visit(AstSymbol *node);
  void visit(AstVariable *node);
  void visit(BinaryOperator *node);
  void visit(UnaryOperator *node);
  void visit(BuiltInFunc *node);

private:
  FILE *out;
};

void CodeGenerator::generate_latex(FILE *out) {
  SymbolTable *symbol_table = SymbolTable::get_instance();

  fputs(
    "\\begin{equation}\n"
    "\\begin{split}\n",
    out
  );

  /* Print latex formated equations */
  for (const auto& sym : symbol_table->get_symbols()) {
    fprintf(out, "\t\\dot %s &= ", sym.name.c_str());

    LatexPrinter print_visitor{out};
    sym.equation->accept(&print_visitor);
    fputs("\\\\\n", out);
  }

  fputs(
    "\\end{split}\n"
    "\\end{equation}",
    out
  );
}


/* LatexPrinter */

void LatexPrinter::visit(AstNumber *node) {
	fprintf(out, "%g", node->value);
}

void LatexPrinter::visit(AstSymbol *node) {
	fprintf(out, "%s", node->name.c_str());
}

void LatexPrinter::visit(AstVariable *node) {
	fprintf(out, "%s", node->name.c_str());
}

void LatexPrinter::visit(BinaryOperator *node) {
  switch (node->operat) {
  case '+':
  case '-':
    fputs("\\left(", out);
    node->left->accept(this);
    fprintf(out, "%c", node->operat);
    node->right->accept(this);
    fputs("\\right)", out);
    break;
  case '*':
    node->left->accept(this);
    node->right->accept(this);
    break;
  case '/':
    fputs("\\frac{", out);
    node->left->accept(this);
    fputs("}{", out);
    node->right->accept(this);
    fputs("}", out);
    break;
  case '^':
    node->left->accept(this);
    fputs("^{", out);
    node->right->accept(this);
    fputs("}", out);
    break;
  default:
    break;
	}
}

void LatexPrinter::visit(UnaryOperator *node) {
	if (node->operat == 'M') {
		fputs("\\left(-", out);
		node->operand->accept(this);
    fputs("\\right)", out);
	}
	else if (node->operat == 'A') {
		fprintf(out, "%s","\\left|");
		node->operand->accept(this);
		fprintf(out, "%s","\\right|");
	}
}

void LatexPrinter::visit(BuiltInFunc *node) {
  if (node->name == "sqrt") {
    fputs("\\sqrt{", out);
    node->argument->accept(this);
    fputs("}", out);
  } else if (node->name == "exp") {
    fputs("e^{", out);
    node->argument->accept(this);
    fputs("}", out);
  } else {
    fprintf(out, "\\%s", node->name.c_str()); // Backslash in front of sin and log etc.
    fputs("(", out);
    node->argument->accept(this);
    fputs(")", out);
  }
}
