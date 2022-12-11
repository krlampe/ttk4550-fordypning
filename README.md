# ttk4550-fordypning

TTK4550 Specialization project fall 2022

## Dependencies

### C and C++ compilers

gcc and g++

### flex and bison

Linux: `sudo apt install flex bison`

Windows: Install [Windows Subsystem for Linux](https://learn.microsoft.com/en-us/windows/wsl/setup/environment), then `sudo apt install build-essential flex bison`.

## Demo 4

Build: **make**

Run: **./sim-gen [mode] [input] [output]**

The mode can be either "matlab" or "latex". If mode is omitted, "matlab" is the default and input and output can't be specified.

The input and output are filenames.

If output is omitted, "ode_sim.m" is default matlab and "ode.tex" is default for latex.

If input also is omitted, "ode.txt" will be used and is assumed to exist.

**make run** will build and run with the default parameters.

## Demo 3

Build: **make**

Run: **./sim-gen [input] [output]**

If output is omitted, "ode_sim.m" will be used. If input also is omitted, "ode.txt" will be used, and is assumed to exist.

**make run** will build and run with the default filenames.

## Demo 1 and 2

Build: **make**

Run: **./ode-parser**

Equations are typed on the command line. Output is written to stdout.
