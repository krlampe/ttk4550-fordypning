#ifndef DOX_H
#define DOX_H

#include <string>
#include <vector>

/*
* Design of Experiements (DOX) data strucuture
* 
* Initial values and parameter values are the only required input from
* the user, the rest have default values. The input from the user is not
* checked, but assumed to be valid. If no time step is suggested by the user,
* none is given to the solver, which then will choose itself.
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