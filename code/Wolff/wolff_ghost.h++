
#ifndef WOLF_GHOST_HPP
#define WOLF_GHOST_HPP

#include <Heisenberg.h++>
#include <algorithm>
#include <cmath>

/*
wolff algorithm for the Heisenberg3D model

/ @brief
/ @param lattice our 3d lattice, where to perform the simulation on
/ @return if the procedure succeds
/ @exception may fail
*/
flt wolff_ghost(Lattice& lattice, flt T, flt J, flt MaxTime, uint MaxSteps);

#endif