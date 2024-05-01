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
                F64 T/*temperature*/, 
                F64 J/*interaction Strength*/,
                F64 maxTimeSeconds, 
                uint maxSteps, Spin h = Spin(0.0, 0.0, 0.0), 
                Spin k = Spin(0.0, 0.0, 0.0),
                MoveType moveType=MoveType::SmallStep);

#endif