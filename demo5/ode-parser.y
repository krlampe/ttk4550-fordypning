/*
* Ordinary Differential Equations (ODE) parser
*/

%define api.prefix {ode}

%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.hh"

int odelex(void);
void odeerror(const char *msg);
extern int odelineno;
extern char *odetext;
%}

%union {
  struct AST *node;
  char *name;
  double number;
}

%token DECLARATION
%token <name> PARAMETER

%token <name> SYMBOL
%token <number> NUMBER
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
| eqlist DECLARATION paramlist {}
| eqlist SYMBOL D '=' exp   {
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
   | NUMBER               { $$ = new AstNumber{$1}; }
   | T                    { $$ = new AstVariable{"t"}; }
   | SYMBOL               { $$ = new AstSymbol{$1}; free($1); }
   | PARAMETER            { $$ = new AstVariable{$1}; free($1); }
   | CALL '(' exp ')'     { $$ = new BuiltInFunc{$1, $3}; free($1); }
   ;

paramlist: SYMBOL         { SymbolTable::get_instance()->add_param($1); free($1); }
| paramlist ',' SYMBOL    { SymbolTable::get_instance()->add_param($3); free($3); }
| PARAMETER               { free($1); }
| paramlist ',' PARAMETER { free($3); }
;
%%

void odeerror(const char *msg) {
  fprintf(stderr, "ode line %d: %s at '%s'\n", odelineno, msg, odetext);
}
