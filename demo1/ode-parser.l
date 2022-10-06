%option noyywrap nodefault noinput nounput

/* float exponent */
EXP ([Ee][-+]?[0-9]+)

%%
 /* single character operators */
"=" { puts("EQUAL"); }
"+" { puts("PLUS"); }
"-" { puts("MINUS"); }
"*" { puts("MULTIPLY"); }
"/" { puts("DIVIDE"); }
"^" { puts("POWER"); }
"(" { puts("OP"); }
")" { puts("CP"); }
"|" { puts("ABS"); }

 /* reserved variable  */
"t" { puts("Independent variable t"); }

 /* built-in functions */
"sqrt"  { puts("BUILT-IN sqrt"); }
"exp"   { puts("BUILT-IN exp"); }
"log"   { puts("BUILT-IN log"); }
"sin"   { puts("BUILT-IN sin"); }
"cos"   { puts("BUILT-IN cos"); }

 /* symbols */
[a-zA-Z][a-zA-Z0-9]* { printf("SYMBOL %s\n", yytext); }
[a-zA-Z][a-zA-Z0-9]*"'" { printf("DERIVATIVE %s\n", yytext); }

 /* decimal numbers */
[0-9]+"."[0-9]*{EXP}? |
"."?[0-9]+{EXP}? { printf("NUMBER %s\n", yytext); }

"//".*  { /* ignore comments */ }

\n    { puts("NEWLINE"); }

\\\n  { /* ignore line continuation */ }

[ \t]	{ /* ignore whitespace */ }

.	{ printf("Unknown character %c\n", *yytext); }
%%

int main() {
  return yylex();
}
