#ifndef ADAPTIVE_METROPOLIS_HPP
#define ADAPTIVE_METROPOLIS_HPP

#include <Base.h++>
#include <Lattice3d.h++>
#include <Spin.h++>


/*
ado algorithm for the Heisenber3D model

/ @brief
/ @param lattice our 3d lattice, where to perform the simulation on
/ @return if the procedure succeds
/ @exception may fail
*/
bool adaptive_metropolis(Lattice & lattice);

#endif