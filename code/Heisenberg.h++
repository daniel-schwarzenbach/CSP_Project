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
#include <Lattice3d.h++>

extern template class Lattice3d<SpinVector>;
extern template class Lattice3d<bool>;
/*
Lattice class
- with acces operator: (x,y,z)
- with boundry conditions: set_boundry_conditions()
- Lx(), Ly(), Lz() // size of the lattice
- comunicate_ghost_cells() // does nothing in Serial
*/
using Lattice = Lattice3d<Spin>;


#ifdef WITH_OPENMP

#include <omp.h>

// #define PARALLEL_FOR_COLLAPSE_3 #pragma omp parallel for
// #define PARALLEL_FOR_ #pragma omp parallel for
// #define REDUCTION_SUM #pragma omp reduction(+ : sum)
// #define ATOMIC #pragma omp atomic
// #define CRITICAL #pragma omp critical
// #define COLLAPSE_3_SUM #pragma omp parallel for collapse(3) reduction(+: sum)

#else // NOT_WITH_OPENMP

uint omp_get_num_threads();
void omp_set_num_threads(int);
#define PARALLEL_FOR  // theoretical parallel
#define REDUCTION_SUM // theretical reduction
#define ATOMIC        // theoretical atomic
#define CRITICAL      // theoretical critical

#endif // WITH_OPENMP



#define PARALLEL_FOR  // theoretical parallel
#define REDUCTION_SUM // theretical reduction
#define ATOMIC        // theoretical atomic
#define CRITICAL      // theoretical critical


// boltzmann constant
static constexpr flt _kB_ = 1.0; // 1.38064852e-23;

/*
Definition of thermodynamic beta

/ @param T: temperature
/ @return 1 / (T * _kB_)
*/
flt Beta(flt T);

/*
calculate the mean value of a vector

/ @brief
/ @param array: vector of Floats
/ @return mean value of the vector: m = 1/m.size() * ∑ m[i]
*/
template <typename Float>
Float mean(Array<Float> const& array);
/*
calculate the variance of a vector

/ @brief
/ @param array: vector of Floats
/ @return mean value of the vector: m = 1/m.size() * ∑ m[i]
*/
template <typename Float>
Float variance(Array<Float> const& array);

/*
- alternative to to_string
- only gives 3 digits after the comma and removes zeros

*/
string to_str(flt const& value);

extern template f32 mean<f32>(Array<f32> const&);
extern template f64 mean<f64>(Array<f64> const&);
extern template f32 variance<f32>(Array<f32> const&);
extern template f64 variance<f64>(Array<f64> const&);

#endif // __HEISENBERG_H__