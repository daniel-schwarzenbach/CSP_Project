#ifndef ENERGY_DIFF_H
#define ENERGY_DIFF_H

#include <Heisenberg.h++>

flt calculateEnergyDiff(Lattice const &lattice, int x, int y, int z,
                        Spin const &oldSpin, Spin const &newSpin,
                        flt J /*interactionStrength*/);

#endif // ENERGY_DIFF_H