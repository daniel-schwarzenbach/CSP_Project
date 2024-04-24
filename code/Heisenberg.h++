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
using Spin = SpinCartesian;
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
using Lattice = LatticeSerial<Spin>;

#define PARALLEL_FOR  // theoretical parallel
#define REDUCTION_SUM // theretical reduction
#define ATOMIC        // theoretical atomic
#define CRITICAL      // theoretical critical

#endif // IPPL_H

#endif // __HEISENBERG_H__



/*Define the Boltzmann constant*/
int power = -23.0;
flt kB = 1.38064852 * pow(10.0, power);

/*
Definition of thermodynamic beta
*/

flt Beta(flt T) {
    return 1/(kB * T);
};

/*
Given an array[] of floats with a length l, this function returns the average value
*/
flt average(flt array[], int l) {
    double sum = 0;
    for (int i = 0; i < n; i++)
        sum += array[i];

    return sum / l;
};

/*
Given an array[] of floatswith a length l and mean value avg, this function returns the square value of the variance:
return: var^2 = 1/l * 𝚺_{i=1}^N (array[i] - mean)^2
*/
flt variance(flt array[], int l, flt mean) {
    double return_variance = 0;
    for (int i = 0; i < n; i++)
        return_variance += pow(array[i] - mean, 2);

    return return_variance / (l - 1);
};
