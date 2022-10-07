%{
#include <stdlib.h>
#include <stdio.h>
int yylex(void);
int yyerror(char *s);
%}

%union {
  double number;
  char name[10];
}

%token <name> FUNCTION
%token <name> T
%token <number> COEFFICIENT
%token D
%token EOL

%%
equation: derivative '=' exp EOL { puts("Successfully parsed equation"); exit(0); } ;

derivative: FUNCTION D { printf("Derivative: %s'\n", $1); } ;

exp: term {}
| exp '+' term { puts("Add"); }
| exp '-' term { puts("Subtract"); }
;

term: factor {}
| term '*' factor { puts("Multiply"); }
| term '/' factor { puts("Divide"); }
;

factor: COEFFICIENT { printf("Factor: %g\n", $1); }
| T { printf("Factor: independent variable %s\n", $1); }
| FUNCTION { printf("Factor: %s\n", $1); }
| '(' exp ')' { puts("Parenthesis"); }
| '-' factor { puts("Unary minus"); }
;
%%

int main() {
  return yyparse();
}

int yyerror(char *s) {
	fprintf(stderr, "error: %s\n", s);
	return 0;
}