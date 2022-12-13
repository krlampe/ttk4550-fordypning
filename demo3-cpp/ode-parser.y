%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.hh"

int yylex(void);
void yyerror(const char *msg);
extern int yylineno;
extern char *yytext;
%}

%union {
  struct AST *node;
  char *name;
  double number;
}

%token <name> FUNCTION
%token <number> COEFFICIENT
%token <name> CALL
%token T
%token D
%token EOL

%left '+' '-'
%left '*' '/'
%nonassoc UMINUS
%right '^'

%type <node> exp

%%
eqlist:
| eqlist EOL
| eqlist FUNCTION D '=' exp {
                              SymbolTable::get_instance()->add_symbol($2, $5);
                              free($2);
                            }
;

exp: exp '+' exp          { $$ = new BinaryOperator{'+', $1, $3}; }
   | exp '-' exp          { $$ = new BinaryOperator{'-', $1, $3}; }
   | exp '*' exp          { $$ = new BinaryOperator{'*', $1, $3}; }
   | exp '/' exp          { $$ = new BinaryOperator{'/', $1, $3}; }
   | '-' exp %prec UMINUS { $$ = new UnaryOperator{'M', $2}; }
   | exp '^' exp          { $$ = new BinaryOperator{'^', $1, $3}; }
   | '|' exp '|'          { $$ = new UnaryOperator{'A', $2}; }
   | '(' exp ')'          { $$ = $2; }
   | COEFFICIENT          { $$ = new AstNumber{$1}; }
   | T                    { $$ = new AstVariable{"t"}; }
   | FUNCTION             { $$ = new AstSymbol{$1}; free($1); }
   | CALL '(' exp ')'     { $$ = new BuiltInFunc{$1, $3}; free($1); }
   ;

%%

void yyerror(const char *msg) {
  fprintf(stderr, "%d: %s at '%s'\n", yylineno, msg, yytext);
}
