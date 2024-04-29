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

#ifdef WITH_OPENMP

#include <omp.h>

// #define PARALLEL_FOR_COLLAPSE_3 #pragma omp parallel for
// #define PARALLEL_FOR_ #pragma omp parallel for
// #define REDUCTION_SUM #pragma omp reduction(+ : sum)
// #define ATOMIC #pragma omp atomic
// #define CRITICAL #pragma omp critical
// #define COLLAPSE_3_SUM #pragma omp parallel for collapse(3) reduction(+: sum)

#else // NOT_WITH_OPENMP

uint omp_get_num_threads()
    return 1;
}
#define PARALLEL_FOR  // theoretical parallel
#define REDUCTION_SUM // theretical reduction
#define ATOMIC        // theoretical atomic
#define CRITICAL      // theoretical critical

#endif // WITH_OPENMP

#include <Lattice3d.h++>
/*
Lattice class
- with acces operator: (x,y,z)
- with boundry conditions: set_boundry_conditions()
- Lx(), Ly(), Lz() // size of the lattice
- comunicate_ghost_cells() // does nothing in Serial
*/
using Lattice = Lattice3d<SpinVector>;


// boltzmann constant
static constexpr F64 _kB_ = 1.0;//1.38064852e-23;

/*
Definition of thermodynamic beta

/ @param T: temperature in K
*/
static F64 Beta(F64 T)
{
    return 1. / (_kB_ * T);
};

/*
calculate the mean value of a vector

/ @brief
/ @param array: vector of Floats
/ @return mean value of the vector: m = 1/m.size() * ∑ m[i]
*/
template <typename Float>
static F64 mean(Array<Float> array)
{
    Float sum = 0;
    uint n = array.size();
    #pragma omp parallel for collapse(1) reduction(+: sum)
    for (uint i = 0; i < n; i++)
        sum += array[i];
    return sum / n;
};

/*
calculate the variance of a vector

/ @brief
/ @param array: vector of Floats
/ @return mean value of the vector: m = 1/m.size() * ∑ m[i]
*/
template <typename Float>
F64 variance(Array<Float> array)
{
    Float mean = mean(array);
    uint n = array.size();
    double sum = 0;
    #pragma omp parallel for collapse(1) reduction(+: sum)
    for (uint i = 0; i < n; i++)
        sum += pow(array[i] - mean, 2);
    return sum / F64(n);
};

#endif // __HEISENBERG_H__