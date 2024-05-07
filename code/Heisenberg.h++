#ifndef __HEISENBERG_H__
#define __HEISENBERG_H__

#include <Base.h++>
#include <Spin.h++>
#include <Lattice3D.h++>




extern template class Lattice3D<Spin>;
extern template class Lattice3D<bool>;
/*
Lattice class
- with acces operator: (x,y,z)
- with boundry conditions: set_boundry_conditions()
- Lx(), Ly(), Lz() // size of the lattice
*/
using Lattice = Lattice3D<Spin>;

#ifdef WITH_OPENMP

#include <omp.h>

#else                 // NOT_WITH_OPENMP

uint omp_get_num_threads();
void omp_set_num_threads(int);
#define PARALLEL_FOR  // theoretical parallel
#define REDUCTION_SUM // theretical reduction
#define ATOMIC        // theoretical atomic
#define CRITICAL      // theoretical critical

#endif // WITH_OPENMP

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
Float mean(Array<Float> const &array);
/*
calculate the variance of a vector

/ @brief
/ @param array: vector of Floats
/ @return mean value of the vector: m = 1/m.size() * ∑ m[i]
*/
template <typename Float>
Float variance(Array<Float> const &array);

/*
converts t -> u

/ @brief
/ @param t: to convert from
/ @param u: to convert to
*/
template <typename T, typename U>
void convert_types(T const& t, U& u){
    u = t;
}


template <typename T, typename U>
Array<U> convert_array(Array<T> const &array){
    Adress size = array.size();
    Array<U> out(size);
    for (Adress i = 0; i < size; ++i){
        convert_types(array[i], out[i]);
    }
    return out;
}

/*
- alternative to to_string
- only gives digits without zeros after the comma

*/
string to_str(flt const &value);

extern template f32 mean<f32>(Array<f32> const &);
extern template f64 mean<f64>(Array<f64> const &);
extern template f32 variance<f32>(Array<f32> const &);
extern template f64 variance<f64>(Array<f64> const &);

#endif // __HEISENBERG_H__