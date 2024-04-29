#ifndef __HEISENBERG_H__
#define __HEISENBERG_H__

#include <Base.h++>
#include <Spin.h++>
#include <Eigen.h++>

/*
Spin class
- with scalar product: |
- with standard product: *
- with addition operator: +
- with subtraction operator: -
- with normalization: normalize()
- with output operator: <<
- with compareration operator: ==
- with assignment operator: =
*/
using Spin = SpinVector;
#define LAMBDA [=]

#ifdef IPPL_H

#include <LatticeIppl.h++>
using Lattice = LatticeIppl<Spin>;

#define PARALLEL_FOR #pragma omp parallel for
#define REDUCTION_SUM #pragma omp reduction(+ : sum)
#define ATOMIC #pragma omp atomic
#define CRITICAL #pragma omp critical

#else // without ippl

#include <LatticeSerial.h++>
/*
Lattice class
- with acces operator: (x,y,z)
- with boundry conditions: set_boundry_conditions()
- Lx(), Ly(), Lz() // size of the lattice
- comunicate_ghost_cells() // does nothing in Serial
*/
using Lattice = Lattice3d<Spin>;

#define PARALLEL_FOR  // theoretical parallel
#define REDUCTION_SUM // theretical reduction
#define ATOMIC        // theoretical atomic
#define CRITICAL      // theoretical critical

#endif // IPPL_H

/*Define the Boltzmann constant*/
static constexpr double kB = 1;

/*
Definition of thermodynamic beta

/ @param T: temperature
*/
static double Beta(double T)
{
    return 1. / (kB * T);
};

#endif // __HEISENBERG_H__