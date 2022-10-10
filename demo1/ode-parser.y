%{
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
%token <name> CALL

%left '+' '-'
%left '*' '/'
%nonassoc UMINUS
%right '^'

%%
eqlist: /* nothing */
| eqlist equation EOL { puts("Successfully parsed equation"); }
| eqlist EOL          {} /* blank line or a comment */
;

equation: derivative '=' exp {} ;

derivative: FUNCTION D { printf("Derivative: %s'\n", $1); } ;

exp: exp '+' exp          { puts("Add"); }
   | exp '-' exp          { puts("Subtract"); }
   | exp '*' exp          { puts("Multiply"); }
   | exp '/' exp          { puts("Divide"); }
   | '-' exp %prec UMINUS { puts("Unary minus"); }
   | exp '^' exp          { puts("Exponent"); }
   | '|' exp '|'          { puts("Absolute value"); }
   | '(' exp ')'          { puts("Parenthesis"); }
   | FUNCTION             { printf("Function %s\n", $1); }
   | COEFFICIENT          { printf("Coefficient %g\n", $1); }
   | T                    { printf("Independent variable %s\n", $1); }
   | CALL '(' exp ')'     { printf("Built-in function call %s\n", $1); }
   ;
%%

int main() {
  return yyparse();
}

int yyerror(char *s) {
	fprintf(stderr, "error: %s\n", s);
	return 0;
}
