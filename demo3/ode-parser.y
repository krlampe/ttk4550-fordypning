%{
#include <stdio.h>
#include <string.h>
#include "ast.h"
#include "matlab.h"

int yylex(void);
void yyerror(const char *msg);
extern FILE *yyin;
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

%type <node> exp

%%
eqlist:
| eqlist EOL
| eqlist FUNCTION D '=' exp { 
                              struct ast_node *root = new_ast_node('=', new_ast_symbol('D', $2), $5);
                              symbol_table_add($2, root);
                              free($2);
                            }
;

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
  const char *inputfile_name = "ode.txt";
  if (argc > 1) {
    inputfile_name = argv[1];
  }
  yyin = fopen(inputfile_name, "r");
  if(!yyin) {
    perror(inputfile_name);
    return 1;
  }

  const char *outputfile_name = "ode_sim.m";
  if (argc > 2) {
    outputfile_name = argv[2];
  }
  outputfile = fopen(outputfile_name, "w");
  if(!outputfile) {
    perror(outputfile_name);
    return 1;
  }

  yyparse();

  matlab_generate(outputfile);

  symbol_table_free();

  fclose(yyin);
  fclose(outputfile);

  return 0;
}

void yyerror(const char *msg) {
  fprintf(stderr, "%d: %s at '%s'\n", yylineno, msg, yytext);
}
