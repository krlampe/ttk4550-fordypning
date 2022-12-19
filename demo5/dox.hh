#ifndef DOX_H
#define DOX_H

#include <string>
#include <vector>

/*
* Design of Experiements (DOX) data strucuture
* 
* Initial values values are the only required input from the user, for
* the simulator to work, the rest have default values. The input from the user
* is not checked, but assumed to be valid. If no initial time step is suggested
* by the user, none is given to the solver, which then will choose ab by itself.
*/
struct DOX {
  std::string solver = "ode45";

  struct {
    double start = 0;
    double end = 1;
  } interval;

  double time_step = 0;
  std::string plot_style = "-";

  std::vector<double> initial_values;
};

/* Global DOX, defined and filled by dox-parser, thus available after doxparse() */
extern DOX dox;

#endif /* DOX_H */