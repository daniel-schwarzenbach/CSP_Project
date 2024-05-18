#ifndef __HEISENBERG_H__
#define __HEISENBERG_H__
/*
    includes all important files for the algorithms and adds important
    definitions
*/

#include <Base.h++>
#include <Array.h++>
#include <Spin.h++>
#include <Lattice3D.h++>

extern template class Lattice3D<Spin>;
extern template class Lattice3D<u8>;
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
- alternative to to_string
- only gives digits without zeros after the comma
1.200000 -> 1.2
*/
string to_str(flt const &value);

#endif // __HEISENBERG_H__