#ifndef MATLAB_H
#define MATLAB_H

#include <stdio.h>

/**
 * Generate a matlab simulator to the file out.
 * Uses the symbol table with equations (ASTs) defined in "ast.hh".
*/
namespace Matlab {
  void generate(FILE *outputfile);
}

#endif /* MATLAB_H */