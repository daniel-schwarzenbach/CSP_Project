#ifndef __HEISENBERG_H__
#define __HEISENBERG_H__

#include <Base.h++>
#include <Spin.h++>

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

#else // without ippl

    #include <LatticeSerial.h++>
    using Lattice = LatticeSerial<Spin>;

#endif // IPPL_H



#endif // __HEISENBERG_H__