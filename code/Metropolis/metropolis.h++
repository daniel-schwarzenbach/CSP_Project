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

/ @param lattice: our 3d lattice, where to perform the simulation on
/ @param T: temperature
/ @param J: spin interaction strenth
/ @param maxTimeSeconds: the maximal time the simulation will run
/ @param maxSteps: the maximal number of steps the simulation will run
/ @param h: the magnetization vector of the external field
/ @param k: anisotropy vector
/ @param moveType: the steping the algorithm will do
/ @return if the procedure succeds
*/
bool metropolis(Lattice &lattice,
                flt const& T /*temperature*/,
                flt const& J /*interaction Strength*/,
                flt const& maxTimeSeconds,
                uint const& maxSteps,
                Spin const& h = {0,0,0},
                Spin const& k = {0,0,0},
                MoveType const& moveType = MoveType::SmallStep);

/*
metropolis algorithm for the Heisenberg 3D model

/ @param lattice: our 3d lattice, where to perform the simulation on
/ @param T: temperature
/ @param J: spin interaction strenth
/ @param maxTimeSeconds: the maximal time the simulation will run
/ @param maxSteps: the maximal number of steps the simulation will run
/ @param h: the magnetization vector of the external field
/ @param k: anisotropy vector
/ @param moveType: the steping the algorithm will do
/ @return if the procedure succeds
*/
bool metropolis_omp(Lattice &lattice,
                    flt const& T /*temperature*/,
                    flt const& J /*interaction Strength*/,
                    flt const& maxTimeSeconds,
                    uint const& maxSteps,
                    Spin const& h = {0,0,0},
                    Spin const& k = {0,0,0},
                    MoveType const& moveType = MoveType::SmallStep);

/*
metropolis algorithm for the Heisenberg 3D model with addaptive step
sizes

/ @param lattice: our 3d lattice, where to perform the simulation on
/ @param T: temperature
/ @param J: spin interaction strenth
/ @param maxTimeSeconds: the maximal time the simulation will run
/ @param maxSteps: the maximal number of steps the simulation will run
/ @param h: the magnetization vector of the external field
/ @param k: anisotropy vector
/ @param maxFactor: the maximal step size
/ @return if the procedure succeds
*/
bool adaptive_metropolis(   Lattice &lattice,
                            flt const& T /*temperature*/,
                            flt const& J /*interaction Strength*/,
                            flt const& maxTimeSeconds,
                            uint const& maxSteps,
                            Spin const& h = {0,0,0},
                            Spin const& k = {0,0,0},
                            flt const& maxFactor = 60.0);

#endif
