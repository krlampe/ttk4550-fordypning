# ttk4550-fordypning

TTK4550 Specialization project fall 2022

## Dependencies

### C and C++ compilers

gcc and g++

### flex and bison

Linux: `sudo apt install flex bison`

Windows: Install [Windows Subsystem for Linux](https://learn.microsoft.com/en-us/windows/wsl/setup/environment), then `sudo apt install build-essential flex bison`.

## Demo 5

Build: **make**

Run Matlab mode: **./sim-gen matlab [output] [input ode] [input dox]**

Run LaTeX mode: **./sim-gen latex [output] [input ode]**

The input and output are filenames.

If output is omitted, "ode_sim.m" is default for matlab and "ode.tex" is default for latex.

If input also is omitted, "ode.txt" and "dox.txt" will be used and is assumed to exist. 

DOX: Initial values and parameter values are the only required input from the user, the rest have default values. The input from the user is not checked, but assumed to be valid. If no time step is suggested by the user, none is given to the solver, which then will choose itself.

### Example ODE input
// Van der Pol oscillator

params m

x_1' = x_2

x_2' = m*(1-x_1^2)*x_2 - x_1

### Example DOX input

m = 0

initial_values = [0, 2]

interval = [0, 30]

solver = ode23

plot_style = "--x"

time_step = 0

time_step = 1e-3


## Demo 4

Build: **make**

Run: **./sim-gen [mode] [output] [input]**

The mode can be either "matlab" or "latex". If mode is omitted, "matlab" is the default and input and output can't be specified.

The input and output are filenames.

If output is omitted, "ode_sim.m" is default for matlab and "ode.tex" is default for latex.

If input also is omitted, "ode.txt" will be used and is assumed to exist.

**make run** will build and run with the default parameters.

## Demo 3

Build: **make**

Run: **./sim-gen [output] [input]**

If output is omitted, "ode_sim.m" will be used. If input also is omitted, "ode.txt" will be used, and is assumed to exist.

**make run** will build and run with the default filenames.

## Demo 1 and 2

Build: **make**

Run: **./ode-parser**

Equations are typed on the command line. Output is written to stdout.
