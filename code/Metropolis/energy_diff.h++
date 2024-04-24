#ifndef ENERGY_DIFF_H
#define ENERGY_DIFF_H

#include <Heisenberg.h++>
#include <cmath> // Is it really needed? Check later

float calculateEnergyDiff(Lattice& lattice, int x, int y, int z, Spin& oldSpin, Spin& newSpin);

#endif // ENERGY_DIFF_H