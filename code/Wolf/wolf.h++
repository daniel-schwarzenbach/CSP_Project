#ifndef WOLF_HPP
#define WOLF_HPP

#include <Base.h++>
#include <Lattice3d.h++>
#include <Spin.h++>


/*
wolf algorithm for the Heisenber3D model

/ @brief
/ @param lattice our 3d lattice, where to perform the simulation on
/ @return if the procedure succeds
/ @exception may fail
*/
bool wolf(Lattice& lattice);

#endif