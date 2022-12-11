#ifndef STRATEGY_H
#define STRATEGY_H

#include <stdio.h>

namespace CodeGenerator {

/**
 * Generate a matlab simulator to the file out.
 * Uses the symbol table with equations (ASTs) defined in "ast.hh".
*/
void generate_matlab(FILE *outputfile);

/**
 * Generate Latex equations to the file out.
 * Uses the symbol table with equations (ASTs) defined in "ast.hh".
*/
void generate_latex(FILE *outputfile);

}

#endif /* STRATEGY_H */