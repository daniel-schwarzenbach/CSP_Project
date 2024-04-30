#ifndef ADAPTIVE_METROPOLIS_HPP
#define ADAPTIVE_METROPOLIS_HPP

#include <Heisenberg.h++>
#include <Measure/Timekeeper.h++>
#include <random>
#include <cmath>

/*
ado algorithm for the Heisenber3D model

/ @brief
/ @param lattice our 3d lattice, where to perform the simulation on
/ @return if the procedure succeds
/ @exception may fail
*/
bool adaptive_metropolis(Lattice &lattice, flt T, flt J, flt maxTime,
                         uint maxSteps,
                         flt maxFactor = 60);

#endif