%option prefix="ode"
%option noyywrap nodefault noinput nounput yylineno

%{
#include <string.h>
#include "ode-parser.tab.h"
#include "ast.hh"
void odeerror(const char *msg);
%}

/* float exponent */
EXP ([Ee][-+]?[0-9]+)

%%
 /* single character operators */
"=" |
"+" |
"-" |
"*" |
"/" |
"^" |
"|" |
"(" |
")" |
"," { return yytext[0]; }

 /* prime mark for derivatives */
"'" { return D; }

 /* reserved keyword */
"params" { return DECLARATION; }

 /* reserved variable  */
"t" { return T; }

 /* built-in functions */
"sqrt" |
"exp"  |
"log"  |
"sin"  |
"cos"  { odelval.name = strdup(yytext); return CALL; }

 /* symbols */
[a-zA-Z_][a-zA-Z_0-9]*  {
                          odelval.name = strdup(yytext);
                          if (SymbolTable::get_instance()->lookup_param(yytext)) {
                              return PARAMETER;
                          }
                          return SYMBOL;
                        }

 /* decimal numbers */
[0-9]+"."[0-9]*{EXP}? |
"."?[0-9]+{EXP}? { odelval.number = atof(yytext); return NUMBER; }

"//".*  { /* ignore comments */ }

\n    { return EOL; }

\\\n  { /* ignore line continuation */ }

[ \t]	{ /* ignore whitespace */ }

.	{ odeerror("Unknown character"); }
%%