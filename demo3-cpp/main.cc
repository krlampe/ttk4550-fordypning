#include <stdio.h>
#include <exception>
#include "ast.hh"
#include "matlab.hh"

int yyparse(void);
extern FILE *yyin;

int main(int argc, char *argv[]) {
  try {
    const char *outputfile_name = "ode_sim.m";
    if (argc > 1) {
      outputfile_name = argv[1];
    }
    FILE *outputfile = fopen(outputfile_name, "w");
    if(!outputfile) {
      perror(outputfile_name);
      return 1;
    }

    const char *inputfile_name = "ode.txt";
    if (argc > 2) {
      inputfile_name = argv[2];
    }
    yyin = fopen(inputfile_name, "r");
    if(!yyin) {
      perror(inputfile_name);
      return 1;
    }

    yyparse();

    SymbolTable::get_instance()->symbol_check();

    Matlab::generate(outputfile);

    SymbolTable::get_instance()->free();

    fclose(yyin);
    fclose(outputfile);

    return 0;
  }
  catch (std::exception& e) {
    fprintf(stderr, "exception: %s\n", e.what());
    SymbolTable::get_instance()->free();
    return 1;
  }
  catch (...) {
    fputs("Unknown exception", stderr);
    SymbolTable::get_instance()->free();
    return 2;
  }
}
