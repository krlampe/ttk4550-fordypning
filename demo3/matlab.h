#ifndef MATLAB_H
#define MATLAB_H

#include <stdio.h>

/**
 * Generate a matlab simulator to the file out.
 * Uses the symbol table with equations (ASTs) defined in "ast.h".
*/
void matlab_generate(FILE *out);

#endif /* MATLAB_H */