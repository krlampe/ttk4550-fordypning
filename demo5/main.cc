#include <stdio.h>
#include <string>
#include <exception>
#include <stdexcept>
#include "ast.hh"
#include "strategy.hh"
#include "dox.hh"

int odeparse(void);
extern FILE *odein;

int doxparse(void);
extern FILE *doxin;

int main(int argc, char *argv[]) {
  try {
    void (*strategy)(FILE *outputfile);

    std::string mode = "matlab";
    if (argc > 1) {
      mode = argv[1];
    }
    if (mode == "matlab") {
      strategy = &CodeGenerator::generate_matlab;
    } else if (mode == "latex") {
      strategy = &CodeGenerator::generate_latex;
    } else {
      throw std::runtime_error("no mode specified");
    }

    const char *outputfile_name = "ode_sim.m";
    if (mode == "latex") {
      outputfile_name = "ode.tex";
    }
    if (argc > 2) {
      outputfile_name = argv[2];
    }
    FILE *outputfile = fopen(outputfile_name, "w");
    if(!outputfile) {
      perror(outputfile_name);
      return 1;
    }

    const char *ode_inputfile_name = "ode.txt";
    if (argc > 3) {
      ode_inputfile_name = argv[3];
    }
    odein = fopen(ode_inputfile_name, "r");
    if(!odein) {
      perror(ode_inputfile_name);
      return 1;
    }

    const char *dox_inputfile_name = "dox.txt";
    if (argc > 4) {
      dox_inputfile_name = argv[4];
    }
    doxin = fopen(dox_inputfile_name, "r");
    if(!doxin) {
      perror(dox_inputfile_name);
      return 1;
    }

    odeparse();
    fclose(odein);

    doxparse();
    fclose(doxin);

    SymbolTable::get_instance()->symbol_check();
    strategy(outputfile);

    SymbolTable::get_instance()->free();
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
