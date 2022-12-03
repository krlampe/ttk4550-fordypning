/*
* Abstract syntax tree and symbol table for ordinary differential equations.
*/

#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <string>
#include <vector>

struct AST {
  virtual ~AST() {}
  virtual void symbol_check() const = 0;
  virtual void print(FILE *out) const = 0;
};

struct AstNumber : public AST {
  double value;

  AstNumber(double v) : value{v} {}

  void symbol_check() const;
  void print(FILE *out) const;
};

// Symbol defined by equation
struct AstSymbol : public AST {
  std::string name;

  AstSymbol(std::string n) : name{n} {}

  void symbol_check() const;
  void print(FILE *out) const;
};

// Variable/parameter not defined by equation
struct AstVariable : public AST {
  std::string name;

  AstVariable(std::string n) : name{n} {}

  void symbol_check() const;
  void print(FILE *out) const;
};

struct BinaryOperator : public AST {
  char operat;
  AST *left;
  AST *right;

  BinaryOperator(char op, AST *l, AST *r) : operat{op}, left{l}, right{r} {}
  ~BinaryOperator() { delete left; delete right; }

  void symbol_check() const;
  void print(FILE *out) const;
};

struct UnaryOperator : public AST {
  char operat;
  AST *operand;

  UnaryOperator(char op, AST *oprnd) : operat{op}, operand{oprnd} {}
  ~UnaryOperator() { delete operand; }

  void symbol_check() const;
  void print(FILE *out) const;
};

struct BuiltInFunc : public AST {
  std::string name;
  AST *argument;

  BuiltInFunc(std::string n, AST *a) : name{n}, argument{a} {}
  ~BuiltInFunc() { delete argument; }

  void symbol_check() const;
  void print(FILE *out) const;
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

  Symbol& get_symbol(std::string name);
  int find_symbol(std::string name) const; // return the index of a symbol if it is defined, -1 else
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
};

#endif /* AST_H */