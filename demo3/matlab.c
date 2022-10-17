#include <stdio.h>
#include "matlab.h"
#include "ast.h"

void matlab_generate(ast_node *root, FILE *out){
  fputs("%% ODE: ", out);
  ast_traverse(root, out);
  fputs("\n\n", out);

  char *name = ast_get_LHS(root);

  fprintf(out, "dydt = @(t, %s) ", name);
  ast_traverse_RHS(root, out);

  fputs(
    ";\n"
    "y_0 = 0;\n"
    "[t,y] = ode45(dydt, [0 20], y_0);\n"
    "figure; clf;\n"
    "hold on;\n"
    "plot(t,y(:,1),'-o');\n"
    "hold off;\n"
    "grid on;\n"
    "xlabel('Time t');\n"
    "ylabel('Solution');\n",
    out
  );
  fprintf(out, "legend('%s');\n\n\n", name);
}