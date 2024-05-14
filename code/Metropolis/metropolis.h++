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

/ @param Lattice3D<Spin>: our 3d Lattice3D<Spin>, where to perform 
the simulation on
/ @param T: temperature
/ @param J: spin interaction strenth
/ @param maxTimeSeconds: the maximal time the simulation will run
/ @param maxSteps: the maximal number of steps the simulation will run
/ @param h: the magnetization vector of the external field
/ @param k: anisotropy vector
/ @param moveType: the steping the algorithm will do
/ @return if the procedure succeds
*/
bool metropolis(Lattice3D<Spin> &lattice,
                flt const& T /*temperature*/,
                flt const& J /*interaction Strength*/,
                flt const& maxTimeSeconds,
                u64 const& maxSteps,
                Spin const& h = {0,0,0},
                Spin const& k = {0,0,0},
                MoveType const& moveType = MoveType::SmallStep);

/*
metropolis algorithm for the Heisenberg 3D model

/ @param Lattice3D<Spin>: our 3d Lattice3D<Spin>, where to perform 
the simulation on
/ @param T: temperature
/ @param J: spin interaction strenth
/ @param maxTimeSeconds: the maximal time the simulation will run
/ @param maxSteps: the maximal number of steps the simulation will run
/ @param h: the magnetization vector of the external field
/ @param k: anisotropy vector
/ @param moveType: the steping the algorithm will do
/ @return if the procedure succeds
*/
bool metropolis_omp(Lattice3D<Spin> &lattice,
                    flt const& T /*temperature*/,
                    flt const& J /*interaction Strength*/,
                    flt const& maxTimeSeconds,
                    u64 const& maxSteps,
                    Spin const& h = {0,0,0},
                    Spin const& k = {0,0,0},
                    MoveType const& moveType = MoveType::Addaptive);

/*
metropolis algorithm for the Heisenberg 3D model with addaptive step
sizes

/ @param Lattice3D<Spin>: our 3d Lattice3D<Spin>, where to perform 
the simulation on
/ @param T: temperature
/ @param J: spin interaction strenth
/ @param maxTimeSeconds: the maximal time the simulation will run
/ @param maxSteps: the maximal number of steps the simulation will run
/ @param h: the magnetization vector of the external field
/ @param k: anisotropy vector
/ @param maxFactor: the maximal step size
/ @return if the procedure succeds
*/
bool adaptive_metropolis(   Lattice3D<Spin> &lattice,
                            flt const& T /*temperature*/,
                            flt const& J /*interaction Strength*/,
                            flt const& maxTimeSeconds,
                            u64 const& maxSteps,
                            Spin const& h = {0,0,0},
                            Spin const& k = {0,0,0},
                            flt const& maxFactor = 60.0);

/*
    returns the sigma of the adaptive metropolis
*/
flt get_sigma();

/*
    returns the acceptance rate of the adaptive metropolis
*/
flt get_acceptance_rate();

/*
    returns the sigma of the adaptive metropolis omp
*/
flt get_sigma_omp();

/*
    returns the acceptance rate of the adaptive metropolis omp
*/
flt get_acceptance_rate_omp();


/*
    resets the adaptation data for the adaptive metropolis omp
*/
void restet_adaptive_omp(flt const& sigma = 60, 
                         flt const& acceptanceRate = 0.5,
                         flt const& totalSteps = 0);

/*
    resets the adaptation data for the adaptive metropolis
*/
void restet_adaptive(flt const& sigma = 60, 
                     flt const& acceptanceRate = 0.5,
                     flt const& totalSteps = 0);

#endif
