#ifndef METROPOLIS_HPP
#define METROPOLIS_HPP

#include <Base.h++>
#include <Lattice3d.h++>
#include <Spin.h++>


/*
metropolis algorithm for the Heisenber3D model

/ @brief
/ @param lattice our 3d lattice, where to perform the simulation on
/ @return if the procedure succeds
/ @exception can throw
*/
bool metropolis(Lattice3d<Spin> & lattice);

#endif