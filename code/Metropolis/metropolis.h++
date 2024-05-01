#ifndef METROPOLIS_HPP
#define METROPOLIS_HPP

#include <Measure/Timer.h++>
#include <cmath>
#include <random>
#include <Heisenberg.h++>

enum MoveType
{
    SpinFlip,
    Random,
    SmallStep,
    Addaptive
};

/*
metropolis algorithm for the Heisenberg 3D model

/ @brief
/ @param lattice our 3d lattice, where to perform the simulation on
/ @return if the procedure succeds
/ @exception may fail
*/
bool metropolis(Lattice &lattice,
                flt const& T /*temperature*/,
                flt const& J /*interaction Strength*/,
                flt const& maxTimeSeconds,
                uint const& maxSteps,
                MoveType const& moveType = MoveType::SmallStep);

#endif