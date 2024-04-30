#ifndef ENERGY_DIFF_H
#define ENERGY_DIFF_H

#include <Heisenberg.h++>

F64 calculateEnergyDiff(Lattice const& lattice, int x, int y, int z, 
                          Spin const& oldSpin, Spin const& newSpin, 
                          F64 J /*interactionStrength*/);

#endif // ENERGY_DIFF_H