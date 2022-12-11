#include <stdio.h>
#include <string>
#include "ast.hh"
#include "strategy.hh"

/**
 * Print RHS of differential equations for latex code by visiting ASTs.
 * Pretty printing with minimal usage of parentheses.
 * One simplification: Always use parentheses around unary minus expressions.
 * Thus, only BinaryOperators need extra parentheses.
 * With some more effort, unary minus could have been handled case by case.
 */
class LatexPrettyPrinter : public AstVisitor {
public:
  LatexPrettyPrinter(FILE *outputfile) : out{outputfile} {}

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

    LatexPrettyPrinter print_visitor{out};
    sym.equation->accept(&print_visitor);
    fputs("\\\\\n", out);
  }

  fputs(
    "\\end{split}\n"
    "\\end{equation}",
    out
  );
}


/* LatexPrettyPrinter */

void LatexPrettyPrinter::visit(AstNumber *node) {
	fprintf(out, "%g", node->value);
}

void LatexPrettyPrinter::visit(AstSymbol *node) {
	fprintf(out, "%s", node->name.c_str());
}

void LatexPrettyPrinter::visit(AstVariable *node) {
	fprintf(out, "%s", node->name.c_str());
}

// Looks ahead down the tree to see if extra parentheses are needed
void LatexPrettyPrinter::visit(BinaryOperator *node) {
  switch (node->operat) {
  case '+':
    node->left->accept(this);
    fputs("+", out);
    node->right->accept(this);
    break;

  case '-':
    node->left->accept(this);
    fputs("-", out);

    // Minus is not an associative operator, parentheses might be needed to its right
    if (auto child = dynamic_cast<BinaryOperator*>(node->right)) {
      if (child->operat == '+' || child->operat == '-') {
        fputs("\\left(", out);
        child->accept(this);
        fputs("\\right)", out);
        break;
      }
    }
    node->right->accept(this);
    break;

  case '*':
    // Left operand
    if (auto child = dynamic_cast<BinaryOperator*>(node->left)) {
      // Only + and - has lower precedence
      if (child->operat == '+' || child->operat == '-') {
        fputs("\\left(", out);
        child->accept(this);
        fputs("\\right)", out);
      } else {
        child->accept(this);
      }
    } else {
      node->left->accept(this);
    }

    // Include the * sign only if both operands are numbers.
    if (dynamic_cast<AstNumber*>(node->left) && dynamic_cast<AstNumber*>(node->right)) {
        fputs("\\cdot", out);
    }

    // Right operand
    if (auto child = dynamic_cast<BinaryOperator*>(node->right)) {
      if (child->operat == '+' || child->operat == '-') {
        fputs("\\left(", out);
        child->accept(this);
        fputs("\\right)", out);
      } else {
        child->accept(this);
      }
    } else {
      node->right->accept(this);
    }
    break;

  case '/':
    // Fractions are handled by the frac command
    fputs("\\frac{", out);
    node->left->accept(this);
    fputs("}{", out);
    node->right->accept(this);
    fputs("}", out);
    break;

  case '^':
    // Every operator has lower precedence than the exponent
    if (dynamic_cast<BinaryOperator*>(node->left)) {
      fputs("\\left(", out);
      node->left->accept(this);
      fputs("\\right)", out);
    } else {
      node->left->accept(this);
    }
    // Superscript for the exponent, no parentheses
    fputs("^{", out);
    node->right->accept(this);
    fputs("}", out);
    break;
  default:
    break;
	}
}

void LatexPrettyPrinter::visit(UnaryOperator *node) {
	if (node->operat == 'M') {
    // Use {} to force unary minus in LaTeX
		fputs("\\left({-", out);
    if (auto child = dynamic_cast<BinaryOperator*>(node->operand)) {
      // Only the exponent has higher precedence than unary minus.
      if (child->operat != '^') {
        fputs("\\left(", out);
        child->accept(this);
        fputs("\\right)", out);
        fputs("}\\right)", out);
        return;
      }
    }
		node->operand->accept(this);
    fputs("}\\right)", out);
    return;
	}

  // Absolute value
	if (node->operat == 'A') {
		fprintf(out, "%s","\\left|");
		node->operand->accept(this);
		fprintf(out, "%s","\\right|");
	}
}

void LatexPrettyPrinter::visit(BuiltInFunc *node) {
  if (node->name == "sqrt") {
    fputs("\\sqrt{", out);
    node->argument->accept(this);
    fputs("}", out);
  } else if (node->name == "exp") {
    fputs("e^{", out);
    node->argument->accept(this);
    fputs("}", out);
  } else {
    // Backslash in front of sin and log, etc.
    fprintf(out, "\\%s", node->name.c_str());
    fputs("(", out);
    node->argument->accept(this);
    fputs(")", out);
  }
}
