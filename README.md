# Heisenberg Model in 3D
### Computational Statistical Physics ETH 2024
Authors: Constança Tropa, Mateo Cárdenes Wuttig, Justus Grabowsky, Daniel Schwarzenbach

This it the github repository for our implementation of the Metropolis and Wolff algorithms to simulate the Heisenberg model:
$H_{\text{Heisenberg}} = - J \sum_{\langle i,j \rangle} \vec{\sigma}_i \cdot \vec{\sigma}_j - \vec{h} \cdot \sum_i \vec{\sigma}_i$

Our project is documented in the final report (report.pdf).

## Structure

### Code

Our main code is inside the folder Code

- Base.h++: defines types and  includes most important files
- RNG.h++: a centralized random number generator for the whole program
- Eigen.h++: includes the linalg library Eigen
- Spin.h++: defines our Spins
- Lattice.h++: defines the 3D Lattice
- Heisenberg.h++: wraps upp all headers in one centralized header
- Wolff / : wolff algorithm
    - wolff.h++: defines the wolff algorithms
- Metropolis / : metropolis algorithm
    - metropolis.h++: defines the metropolis algorithms
    - energy_diff.h++: calculates differences in energy of two Spins
- Measure / : contains various measure functions
    - Timer.h++: defines a simple and easy to use stopwatch
    - Observables.h++: defines function to measure the Lattice
    - LoadingBar.h++: implements a small Loading bar for simulations
- Data / : for evaluating, storing, loading data
    - DataHandler.h++: store and load data
    - MPI_Helper.h++: slices up data for each core
    - Plot.h++: includes usefull ploting functions for the lattice
- Simulation / : wraps up the algorithms for testing
    - Simulation.h++: defines universal function lambdas and test_functions

### programms

contains various programs for the Heisenberg3D modell

### buildscripts

contains various build scripts for easy build of the programm

### libraries

- Mathplot++ : C++ ploting

### docs

- includes Euler launch scripts

and other documents

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

## Matplot++-Library

Is auto installed through CMake

[Documentation](https://alandefreitas.github.io/matplotplusplus/)

[Github-Matplot++](https://github.com/alandefreitas/matplotplusplus)
