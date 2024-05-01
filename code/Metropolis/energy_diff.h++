#ifndef ENERGY_DIFF_H
#define ENERGY_DIFF_H

#include <Heisenberg.h++>

F64 calculateEnergyDiff(Lattice& lattice, int x, int y, int z, 
                          Spin& oldSpin, Spin& newSpin, 
                          F64 J, Spin h, Spin k);

#endif // ENERGY_DIFF_H