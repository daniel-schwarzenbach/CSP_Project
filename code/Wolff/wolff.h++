#ifndef WOLF_HPP
#define WOLF_HPP

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
F64 wolff(Lattice& lattice, F64 T, F64 J, F64 MaxTime, u64 MaxSteps, Spin h);

#endif