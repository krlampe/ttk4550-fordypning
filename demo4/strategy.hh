#ifndef STRATEGY_H
#define STRATEGY_H

#include <stdio.h>

class CodeGenerator {
public:
  virtual ~CodeGenerator() {}
  virtual void operator()(FILE *outputfile) const = 0;
};

/**
 * Generate a matlab simulator to the file out.
 * Uses the symbol table with equations (ASTs) defined in "ast.hh".
*/
class MatlabGenerator : public CodeGenerator {
public:
  void operator()(FILE *outputfile) const override;
};

/**
 * Generate Latex equations to the file out.
 * Uses the symbol table with equations (ASTs) defined in "ast.hh".
*/
class LatexGenerator : public CodeGenerator {
public:
  void operator()(FILE *outputfile) const override;
};

#endif /* STRATEGY_H */