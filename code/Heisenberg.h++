#ifndef __HEISENBERG_H__
#define __HEISENBERG_H__

#include <Base.h++>
#include <Spin.h++>
#include <Eigen.h++>

/*
Spin class
- with scalar product: |
- with output operator: <<
- with compareration operator: ==
- with assignment operator: =
*/
using Spin = SpinXYZ;
#define LAMBDA [=]


#ifdef IPPL_H
    
    #include <LatticeIppl.h++>
    using Lattice = LatticeIppl<Spin>;

    #define PARALLEL_FOR #pragma omp parallel for
    #define REDUCTION_SUM #pragma omp reduction(+:sum)
    #define ATOMIC #pragma omp atomic
    #define CRITICAL #pragma omp critical

#else // without ippl


    #include <LatticeSerial.h++>
    /*
    Lattice class
    - with acces operator: (x,y,z)
    - with boundry conditions: set_boundry_conditions()
    */
    using Lattice = LatticeSerial<Spin>;

    #define PARALLEL_FOR // theoretical parallel
    #define REDUCTION_SUM // theretical reduction
    #define ATOMIC  // theoretical atomic
    #define CRITICAL // theoretical critical

#endif // IPPL_H



#endif // __HEISENBERG_H__