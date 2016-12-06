# Software Model Checking course assignments

This repository contains the assignments for the course "Software
Model Checking" at [TU Vienna](https://tuwien.ac.at). These
assignments are programming exercises, meaning that working code
solutions have to be handed in by the participants.

The code in this repository can be used as a template for the
solution, but participants are free to develop their solutions in any
programming language or framework they choose, provided that

1. The project can be built using publically available tools and
   compilers.
2. The project can be run on a linux machine.

## Building the framework

### Prerequisites

The following software is required to build the framework:

* [CMake](https://cmake.org) is used as a build tool. At least version
  2.6 is required. Most distributions provide binary packages.
* [MathSAT](http://mathsat.fbk.eu) is used as the SMT solver
  backend. Binary packages for Linux, MacOS and Windows are available.

### Compiling

To build the framework, first create a build directory

> mkdir build
> cd build

then generate Makefiles using

> cmake ..

and finally compile the project

> make

### Running

If everything worked out, you should end up with a executable "smc" in
the build directory. It can be executed in bounded-model-checking
(*BMC*) mode using

> ./smc bmc 10 < ../tests/sum.l

or in predicate abstraction mode using

> ./smc pred_abs < ../tests/sum.l
