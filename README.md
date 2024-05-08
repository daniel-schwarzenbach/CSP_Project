# Heisenberg3D modell

Every picture is 500 random steps with metropolis algorithm at T=0.01. 100'000 steps in total.

![metropolis algorithm](./docs/plots/metropolis.gif)

# Table of contents

 1. Introduction
 2. Structure of the code
 4. Algorithms
    - Metropolis
    - Adaptive Metropolis
    - Wolff Algorithm
 5. Results
 6. Contributions
 7. Installation
 8. Libraries

# Introduction

This repository contains the code to simulate the 3D Heisenberg model using
different algorithms. The 3D Heisenberg model is a lattice model that
is used to understand the behavior of magnetic materials under various
conditions. The three algorithms implemented for the simulation are the Wolff
algorithm, the standard Metropolis algorithm and an adaptive version
of the Metropolis algorithm.

In this README file we give instructions for the installation of the
required libraries, provide an overview over the code, describe the
core difference between the three algorithms implemented and give a 
brief overview over the results that can be obtained using our code. 
Finally, we list the contributions.


# Structure of the code 

#### code

Our implementation can be found in the folder 'code'.
The different folders contain the following:


- Data / : for evaluating, storing, loading data
    - DataHandler.h++: store and load data
    - MPI_Helper.h++: slices up data for each core
    - Plot.h++: includes plotting functions for the lattice
- Measure: contains various measure functions
    - Timer.h++: defines a simple and easy to use stopwatch
    - Observables.h++: defines functions to measure the Lattice
        1. get::energy: calculates energy of the current lattice configuration
        2. get::magnetisation: calculates magnetization of the current lattice configuration
    - LoadingBar.h++: implements a Loading bar to track progressof simulations
- Metropolis:
    - metropolis.h++: implements the metroplis algorithms
    - energy_diff.h++: calculates the energy difference between initial and proposed configuration
- Wolff / : wolff algorithm
    - wolff.h++: defines the Wolff algorithm
- Base.h++: types and includes of most important files
- Eigen.h++: includes 'Eigen' for linear algebra operations
- Heisenberg.h++: wraps up all headers in one centralized header
- Lattice.h++: defines 3D lattice class of the Heisenberg model 
- RNG.h++: standardized random number generator used in the whole repository
- Spin.h++: defines our spins and the member functions

#### buildscripts

contains various build scripts

#### docs

contains plots, documents and launch scripts for the euler cluster

#### libraries 

contains all libraries

#### Programs

contains various programs for the Heisenberg3D modell

#### .vscode

contains files for working in VS-Code

# Algorithms

## Metropolis

The Metropolis algorithm explores the phase space of the system by proposing
spin flips, based on a specified trial move, and accepts them based on the
energy difference between the two configurations. We implemented three
different trial moves: the spin flip, the small step and the random step, which
are member functions of the spin class. The implementation allows applying an 
external magnetic field as well as a magnetic anisotropy. More information
about the algorithm can be found [here](https://www.aliquote.org/pub/metropolis-et-al-1953.pdf).

## Adaptive Metropolis

The adaptive Metropolis is an extension of the standard Metropolis algorithm,
that is based on a different trial move, with an adaptive elememt. This 
enhances the sampling efficieny and keeps the acceptance rate close to
fifty percent. As for the standard Metropolis, the implementation allows applying an 
external magnetic field as well as a magnetic anisotropy. A detailed description can be found 
[here](https://iopscience.iop.org/article/10.1088/1361-648X/aaf852/meta).

## Wolff

The Wolff algorithm is a cluster update algorithm, that flips entire cluster
contrary to the Metropolis algorithm, which flips individual spins. The 
cluster is build succesively from a randomly chosen site and flipped at
the end of each step. This approach accelerates the exploration of the 
phase space, especially near critical points, making it ideal for studying
phase transitions. More information can be found [here](https://journals.aps.org/prl/abstract/10.1103/PhysRevLett.62.361).

# Results

The code can be used to demonstrate that all algorithms guide an intial spin
configuration towards thermal equilibrium. Notably, for all algorithms,
the value of the observables at equilibirum agree. Simulations show 
a clear phase transition from a magnetized phase at low temperatures to 
a demagnetized phase at high temperatures and the critical exponents 
can be calculated. Additionally, the effect of an external magnetic
field and spatial anisotropies can be observed.

In the following plot, we can observe a phase transition as shown by the order parameter $M$ for $L = 8$. The data was calculated using the Metropolis algorithm.

<img src="./docs/plots/Magnetisation.png" width="500" height="400">

The time per algorithm step is constant for the Metropolis algorithm, as expected...

<img src="./docs/plots/Time_per_step.png" width="500" height="400">

... however, for the Monte Carlo steps, we observe an increase over several orders of magnitude. This is known as the critical slowing down of the dynamics:

<img src="./docs/plots/Time_per_MC.png" width="500" height="400">

# Contributions

M.C.W. implemented the observables, J.G. implemented the normal and adaptive Metropolis algorithm, C.T. implemented the Wolff algorithm, D.S. and J.G. implemented the lattice and spin classes. J.G. and C.T. implemented the data calculation. D.S. set up and managed the git-repository. M.C.W., J.G. and C.T. planned the calculations, analysed the data, and created the figures. J.G. and C.T. calculated the linear and non-linear correlation times, determined the critical exponents, and measured the algorithmic performance. D.S. assigned calculations on the EULER cluster. M.C.W. drafted and wrote the report with input from all authors. All authors contributed equally to the theory and final presentations.

# Installation

## Mac-Os

First u need to install [Homebrew](https://docs.brew.sh/Installation)

install GCC 11 for c++23
```bash
brew install gcc@11 --without-multilib
brew link gcc@11 --force
```
If you’ve previously installed GCC using Homebrew, you can reinstall it with:
```bash
brew reinstall gcc@11 --without-multilib
```

install cmake
```bash
brew install cmake
```

install eigen
```bash
brew install eigen
```

install openmpi
```bash
brew install openmpi --build-from-source --cc=gcc-11
```

## Linux

install GCC 11 for c++23
```bash
sudo apt-get update
sudo apt-get install g++-11
```
install cmake
```bash
brew install cmake
```

install openmpi on **Debian**
```bash
sudo apt-get install openmpi-bin libopenmpi-dev
```

install openmpi on **other Linux dirtros**
- Download the Open MPI source code from the main Open MPI website.
```bash
# Extract the tarball:
tar xf openmpi-<version>.tar.bz2
cd openmpi-<version>
# Configure and build Open MPI
# Replace <path> with your desired installation directory.
./configure --prefix=<path> [ ...options...]
make [-j N] all
# Install Open MPI:
sudo make install

```

## Windows

We sadly do not support windows itself, but you can install Windows Subsystems for Linux (WSL) and follow the linux installation guide.

[install WSL](https://learn.microsoft.com/en-us/windows/wsl/install)

# Libraries

## Matplot++-Library

Rich plotting library

Is auto installed through CMake


[Documentation](https://alandefreitas.github.io/matplotplusplus/)

[Github-Matplot++](https://github.com/alandefreitas/matplotplusplus)

