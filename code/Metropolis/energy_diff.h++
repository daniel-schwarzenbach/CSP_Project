#ifndef ENERGY_DIFF_H
#define ENERGY_DIFF_H

#include <Heisenberg.h++>
#include <cmath>

float calculateEnergyDiff(Lattice& lattice, int x, int y, int z, 
                    Spin& oldSpin, Spin& newSpin, flt interactionStrength,
                    Eigen::Vector3d H, Eigen::Vector3d k);

#endif // ENERGY_DIFF_H