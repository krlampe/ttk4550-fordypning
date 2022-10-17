#ifndef MATLAB_H
#define MATLAB_H

#include <stdio.h>
#include "ast.h"

void matlab_generate(ast_node *root, FILE *out);

#endif /* MATLAB_H */