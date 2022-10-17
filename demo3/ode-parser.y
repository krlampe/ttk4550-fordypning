%{
#include <stdio.h>
#include <string.h>
#include "ast.h"
#include "matlab.h"

int yylex(void);
void yyerror(const char *msg);
extern int yylineno;
extern char *yytext;

FILE *outputfile;
%}

%union {
  struct ast_node *node;
  char *name;
}

%token <name> FUNCTION
%token <name> T
%token <name> COEFFICIENT
%token D
%token EOL
%token <name> CALL

%left '+' '-'
%left '*' '/'
%nonassoc UMINUS
%right '^'

%type <node> equation derivative exp

%%
eqlist:               { printf("%s","> "); } /* nothing */
| eqlist EOL          { printf("%s","\n> "); } /* blank line or a comment */
| eqlist equation EOL { matlab_generate($2, outputfile); ast_free($2); printf("%s","> "); }
;

equation: derivative '=' exp { $$ = new_ast_node('=', $1, $3); } ;

derivative: FUNCTION D { $$ = new_ast_symbol('D', $1); free($1); } ;

exp: exp '+' exp          { $$ = new_ast_node('+', $1, $3); }
   | exp '-' exp          { $$ = new_ast_node('-', $1, $3); }
   | exp '*' exp          { $$ = new_ast_node('*', $1, $3); }
   | exp '/' exp          { $$ = new_ast_node('/', $1, $3); }
   | '-' exp %prec UMINUS { $$ = new_ast_node('M', $2, NULL); }
   | exp '^' exp          { $$ = new_ast_node('^', $1, $3); }
   | '|' exp '|'          { $$ = new_ast_node('A', $2, NULL); }
   | '(' exp ')'          { $$ = $2; }
   | FUNCTION             { $$ = new_ast_symbol('F', $1); free($1); }
   | COEFFICIENT          { $$ = new_ast_numeral($1); free($1); }
   | T                    { $$ = new_ast_symbol('T', $1); free($1); }
   | CALL '(' exp ')'     { $$ = new_ast_node('C', new_ast_symbol('B', $1), $3); free($1); }
   ;
%%

int main(int argc, char *argv[]) {
  char *outputfile_name = "ode_sim.m";
  if (argc > 1) {
    outputfile_name = argv[1];
  }

  outputfile = fopen(outputfile_name, "w");
  // outputfile = stdout
  yyparse();
  puts("");
  fclose(outputfile);
  return 0;
}

void yyerror(const char *msg) {
  fprintf(stderr, "%d: %s at '%s'\n", yylineno, msg, yytext);
}
