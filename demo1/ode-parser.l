%option noyywrap nodefault noinput nounput

%{
#include "ode-parser.tab.h"
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
"t" { strcpy(yylval.name, yytext); return T; }

 /* built-in functions */
"sqrt" |
"exp"  |
"log"  |
"sin"  |
"cos"  { strcpy(yylval.name, yytext); return CALL; }

 /* symbols */
[a-zA-Z][a-zA-Z0-9]* { strcpy(yylval.name, yytext); return FUNCTION; }

 /* decimal numbers */
[0-9]+"."[0-9]*{EXP}? |
"."?[0-9]+{EXP}? { yylval.number = atof(yytext); return COEFFICIENT; }

"//".*  { /* ignore comments */ }

\n    { return EOL; }

\\\n  { /* ignore line continuation */ }

[ \t]	{ /* ignore whitespace */ }

.	{ printf("Unknown character %c\n", *yytext); }
%%