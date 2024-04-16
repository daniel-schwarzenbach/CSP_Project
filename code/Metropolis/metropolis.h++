#ifndef METROPOLIS_HPP
#define METROPOLIS_HPP

#include <Heisenberg.h++>


/*
metropolis algorithm for the Heisenber3D model

/ @brief
/ @param lattice our 3d lattice, where to perform the simulation on
/ @return if the procedure succeds
/ @exception may fail
*/
bool metropolis(Lattice& lattice, flt T, flt J, flt Time);

#endif