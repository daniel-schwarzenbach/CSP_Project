#ifndef ENERGY_DIFF_H
#define ENERGY_DIFF_H

#include <Heisenberg.h++>

flt calculateEnergyDiff(Lattice3D<Spin> const& lattice, int  const& x, 
                        int const& y,int const& z, 
                        Spin const& oldSpin, Spin const& newSpin, 
                        flt const& J, Spin const& h, Spin const& k);

#endif // ENERGY_DIFF_H