%option noyywrap nodefault noinput nounput yylineno

%{
#include "ode-parser.tab.h"
#include <string.h>
void yyerror(const char *msg);
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
")" { return yytext[0]; }

 /* prime mark for derivatives */
"'" { return D; }

 /* reserved variable  */
"t" { yylval.name = strdup(yytext); return T; }

 /* built-in functions */
"sqrt" |
"exp"  |
"log"  |
"sin"  |
"cos"  { yylval.name = strdup(yytext); return CALL; }

 /* symbols */
[a-zA-Z_][a-zA-Z_0-9]* { yylval.name = strdup(yytext); return FUNCTION; }

 /* decimal numbers */
[0-9]+"."[0-9]*{EXP}? |
"."?[0-9]+{EXP}? { yylval.number = atof(yytext); return COEFFICIENT; }

"//".*  { /* ignore comments */ }

\n    { return EOL; }

\\\n  { /* ignore line continuation */ }

[ \t]	{ /* ignore whitespace */ }

.	{ yyerror("Unknown character"); }
%%