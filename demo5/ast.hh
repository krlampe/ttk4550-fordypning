/*
* Abstract syntax tree and symbol table for ordinary differential equations.
* The datastructures are built by ode-parser, thus available after odeparse().
*/

#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <string>
#include <vector>

struct AstNumber;
struct AstSymbol;
struct AstVariable;
struct BinaryOperator;
struct UnaryOperator;
struct BuiltInFunc;

class AstVisitor {
public:
  virtual void visit(AstNumber *node) = 0;
  virtual void visit(AstSymbol *node) = 0;
  virtual void visit(AstVariable *node) = 0;
  virtual void visit(BinaryOperator *node) = 0;
  virtual void visit(UnaryOperator *node) = 0;
  virtual void visit(BuiltInFunc *node) = 0;
};

struct AST {
  virtual ~AST() {}
  virtual void symbol_check() const = 0;
  virtual void print(FILE *out) const = 0;
  virtual void accept(AstVisitor *visitor) = 0;
};

struct AstNumber : public AST {
  double value;

  AstNumber(double v) : value{v} {}

  void symbol_check() const;
  void print(FILE *out) const;

  void accept(AstVisitor *visitor) { visitor->visit(this); };
};

// Symbol defined by equation
struct AstSymbol : public AST {
  std::string name;

  AstSymbol(std::string n) : name{n} {}

  void symbol_check() const;
  void print(FILE *out) const;

  void accept(AstVisitor *visitor) { visitor->visit(this); };
};

// Variable/parameter not defined by equation
struct AstVariable : public AST {
  std::string name;

  AstVariable(std::string n) : name{n} {}

  void symbol_check() const;
  void print(FILE *out) const;
  
  void accept(AstVisitor *visitor) { visitor->visit(this); };
};

struct BinaryOperator : public AST {
  char operat;
  AST *left;
  AST *right;

  BinaryOperator(char op, AST *l, AST *r) : operat{op}, left{l}, right{r} {}
  ~BinaryOperator() { delete left; delete right; }

  void symbol_check() const;
  void print(FILE *out) const;

  void accept(AstVisitor *visitor) { visitor->visit(this); };
};

struct UnaryOperator : public AST {
  char operat;
  AST *operand;

  UnaryOperator(char op, AST *oprnd) : operat{op}, operand{oprnd} {}
  ~UnaryOperator() { delete operand; }

  void symbol_check() const;
  void print(FILE *out) const;

  void accept(AstVisitor *visitor) { visitor->visit(this); };
};

struct BuiltInFunc : public AST {
  std::string name;
  AST *argument;

  BuiltInFunc(std::string n, AST *a) : name{n}, argument{a} {}
  ~BuiltInFunc() { delete argument; }

  void symbol_check() const;
  void print(FILE *out) const;

  void accept(AstVisitor *visitor) { visitor->visit(this); };
};

// Symbols in the symbol table
struct Symbol {
  std::string name;
  int index;
  AST *equation;
};

/**
 * Symbol table as a Singleton: a single instance with a global access point through get_instance().
 * Stores the ASTs of all the differential equations together with the symbols they define.
 * The index of each symbol corresponds to its index in the system of equations (using 0-based indexing).
 */
class SymbolTable {
public:
  static SymbolTable *get_instance(); // returns the signleton instance, and constructs it if it does not exist

  const Symbol& get_symbol(std::string name) const;
  const std::vector<Symbol>& get_symbols() const { return symbols; }
  int get_nr_of_equations() const { return nr_of_equations; }

  int find_symbol(std::string name) const; // returns the index of a symbol if it is defined, -1 else
  void add_symbol(std::string name, AST *equation); // makes a new entry in the symbol table
  void free(); // deletes all the ASTs and clears the table. The AST destructos frees every node recursively

  void symbol_check() const; // check that all symbols used in equations are defined
  void print_symbols() const; // (debugging tool) prints all the symbol names to stdout

  SymbolTable(const SymbolTable&) = delete; // prevent copying
  SymbolTable& operator=(const SymbolTable&) = delete;

private:
  // Private constructor and static instance to restrict access.
  SymbolTable() {};
  static SymbolTable *singleton;

  std::vector<Symbol> symbols; // the table of symbols
  int nr_of_equations = 0;
};

#endif /* AST_H */