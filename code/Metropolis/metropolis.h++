#ifndef METROPOLIS_HPP
#define METROPOLIS_HPP

#include <Measure/Timekeeper.h++>
#include <cmath>
#include <random>
#include <Heisenberg.h++>

enum class MoveType
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
                flt T /*temperature*/,
                flt J /*interaction Strength*/,
                flt maxTimeSeconds,
                uint maxSteps,
                MoveType moveType = MoveType::SmallStep);

#endif