%option prefix="dox"
%option noyywrap nodefault noinput nounput yylineno

%{
#include "dox-parser.tab.h"
#include <string.h>
void doxerror(const char *msg);
%}

/* float exponent */
EXP ([Ee][-+]?[0-9]+)

%%
 /* single character operators */
"=" |
"-" |
"," |
"[" |
"]" { return yytext[0]; }

 /* variables */
"solver"          { return SOLVER; }
"interval"        { return INTERVAL; }
"time_step"       { return TIMESTEP; }
"plot_style"      { return STYLE; }
"initial_values"  { return INITIALS; }

 /* plot style specifiers: line style followed by marker, both beeing opitonal */
("-"|"--"|":"|"-.")?("o"|"+"|"*"|"."|"x")? { doxlval.str = strdup(yytext); return STYLESPEC; }

 /* symbols */
[a-zA-Z_][a-zA-Z_0-9]* { doxlval.str = strdup(yytext); return SYMBOL; }

 /* decimal numbers */
[0-9]+"."[0-9]*{EXP}? |
"."?[0-9]+{EXP}? { doxlval.number = atof(yytext); return NUMBER; }

"//".*  { /* ignore comments */ }

\"    { /* ignore quotes */ }

\n    { return EOL; }

\\\n  { /* ignore line continuation */ }

[ \t]	{ /* ignore whitespace */ }

.	{ doxerror("Unknown character"); }
%%