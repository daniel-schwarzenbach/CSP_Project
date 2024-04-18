#ifndef WOLF_HPP
#define WOLF_HPP

#include <Heisenberg.h++>
#include <algorithm>
#include <cmath>

/*
wolf algorithm for the Heisenberg3D model

/ @brief
/ @param lattice our 3d lattice, where to perform the simulation on
/ @return if the procedure succeds
/ @exception may fail
*/
flt wolf(Lattice& lattice, flt T, flt Time);

#endif