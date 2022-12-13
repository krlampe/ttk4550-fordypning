/*
* Design of Experiements (DOX) parser
*/

%define api.prefix {dox}

%{
#include <stdio.h>
#include <stdlib.h>
#include "dox.hh"
#include "ast.hh"

int doxlex(void);
void doxerror(const char *msg);
extern int doxlineno;
extern char *doxtext;

// Global design of experiments data, initialized here with default values
DOX dox;
%}

%union {
  char *str;
  double number;
}

%token SOLVER
%token INTERVAL
%token TIMESTEP
%token STYLE
%token INITIALS

%token <str> SYMBOL
%token <str> STYLESPEC
%token <number> NUMBER
%token EOL

%%
statements:
| statements EOL
| statements SYMBOL '=' NUMBER { SymbolTable::get_instance()->set_param($2, $4); free($2); }
| statements SOLVER '=' SYMBOL { dox.solver = $4; free($4); }
| statements INTERVAL '=' '[' NUMBER NUMBER ']' { dox.interval.start = $5; dox.interval.end = $6; }
| statements INTERVAL '=' '[' NUMBER ',' NUMBER ']' { dox.interval.start = $5; dox.interval.end = $7; }
| statements TIMESTEP '=' NUMBER { dox.time_step = $4; }
| statements STYLE '=' STYLESPEC { dox.plot_style = $4; free($4); }
| statements INITIALS '=' '[' numberlist ']' {}
;

numberlist: NUMBER      { dox.initial_values.push_back($1); }
| numberlist ',' NUMBER { dox.initial_values.push_back($3); }
;
%%

void doxerror(const char *msg) {
  fprintf(stderr, "dox line %d: %s at '%s'\n", doxlineno, msg, doxtext);
}
