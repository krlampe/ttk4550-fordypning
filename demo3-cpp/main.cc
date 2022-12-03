#include <stdio.h>
#include <exception>
#include "ast.hh"
#include "matlab.hh"

int yyparse(void);
void yyerror(const char *msg);
extern FILE *yyin;

int main(int argc, char *argv[]) {
  try {
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
    FILE *outputfile = fopen(outputfile_name, "w");
    if(!outputfile) {
      perror(outputfile_name);
      return 1;
    }

    yyparse();

    // matlab::generate(outputfile);

    SymbolTable *symbol_table = SymbolTable::get_instance();
    symbol_table->symbol_check();
    symbol_table->print_symbols();
    symbol_table->free();

    fclose(yyin);
    fclose(outputfile);

    return 0;
  }
  catch (std::exception& e) {
    printf("exception: %s\n", e.what());
    SymbolTable::get_instance()->free();
    return 1;
  }
  catch (...) {
    puts("Unknown exception");
    SymbolTable::get_instance()->free();
    return 2;
  }
}
