#include <stdio.h>
#include <string>
#include <exception>
#include <stdexcept>
#include "ast.hh"
#include "strategy.hh"

int yyparse(void);
void yyerror(const char *msg);
extern FILE *yyin;

int main(int argc, char *argv[]) {
  try {
    CodeGenerator *code_generator = nullptr;

    std::string mode = "matlab";
    if (argc > 1) {
      mode = argv[1];
    }
    if (mode == "matlab") {
      code_generator = new MatlabGenerator();
    } else if (mode == "latex") {
      code_generator = new MatlabGenerator();
    } else {
      throw std::runtime_error("no mode specified");
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

    const char *outputfile_name = "ode_sim.m";
    if (mode == "latex") {
      outputfile_name = "ode.tex";
    }
    if (argc > 3) {
      outputfile_name = argv[3];
    }
    FILE *outputfile = fopen(outputfile_name, "w");
    if(!outputfile) {
      perror(outputfile_name);
      return 1;
    }

    yyparse();

    (*code_generator)(outputfile);
    delete code_generator;

    SymbolTable::get_instance()->free();

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
