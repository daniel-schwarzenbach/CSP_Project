#ifndef __HEISENBERG_H__
#define __HEISENBERG_H__
/*
    includes all important files for the algorithms and adds important
    definitions
*/

#include <Base.h++>
#include <Spin.h++>
#include <Lattice3D.h++>

extern template class Lattice3D<Spin>;
extern template class Lattice3D<bool>;

/*
Lattice that contains Spins: aka. Lattice3D<Spin>
Lx,Ly,Lz have to be powers of 2!
- with acces operator: (x,y,z), ({x,y,z})
- with boundry conditions: set_boundry_conditions() / get_...
- Lx(), Ly(), Lz() // size of the lattice
*/
using Lattice = Lattice3D<Spin>;



#ifdef WITH_OPENMP // definition given by cmake
#include <omp.h>
#else                 // NOT_WITH_OPENMP

// definitions if no omp so that the code still compiles
uint omp_get_num_threads();
void omp_set_num_threads(int);
uint omp_get_thread_num();

#endif // WITH_OPENMP

// boltzmann constant
// normalized with J s.t. J / _kB_ = 1
static constexpr flt _kB_ = 1.0; 

/*
Definition of thermodynamic beta

/ @param T: temperature
/ @return 1 / (T * _kB_)
*/
flt Beta(flt const& T);

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
converts t -> u : important for generic convert_array

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
Push back array
*/
Array<flt>& push_back(Array<flt>& array, 
                        Array<flt> const& to_push_back);

/*
Pushes back each array sub array
*/
Array2D<flt>& push_back(Array2D<flt>& array, 
                        Array2D<flt> const& to_push_back);

/*
- alternative to to_string
- only gives digits without zeros after the comma
1.200000 -> 1.2
*/
string to_str(flt const &value);


// template defitions
extern template f32 mean<f32>(Array<f32> const &);
extern template f64 mean<f64>(Array<f64> const &);
extern template f32 variance<f32>(Array<f32> const &);
extern template f64 variance<f64>(Array<f64> const &);

#endif // __HEISENBERG_H__