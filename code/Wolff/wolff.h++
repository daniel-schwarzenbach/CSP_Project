#ifndef WOLF_HPP
#define WOLF_HPP

#include <Heisenberg.h++>
#include <algorithm>
#include <cmath>
#include <algorithm>

/*
wolff algorithm for the Heisenberg3D model

/ @brief
/ @param lattice our 3d lattice, where to perform the simulation on
/ @return if the procedure succeds
/ @exception may fail
*/
flt wolff(  Lattice &lattice, flt const& T, flt const& J, 
            flt const& MaxTime, u64 const& MaxSteps, Spin const& h);

/*
wolff algorithm for the Heisenberg3D model

/ @brief
/ @param lattice: our Lattice3d<Spin>, where to perform the simulation 
on
/ @param T:
/ @return if the procedure succeds
/ @exception may fail
*/
flt wolff_ghost(Lattice& lattice, flt const& T, flt const& J, 
                flt const& MaxTime, u64 const& MaxSteps, Spin const& h);


/*
wolff algorithm for the Heisenberg3D model that is naivly parallel

/ @param lattice our 3d lattice, where to perform the simulation on
/ @return if the procedure succeds
/ @exception may fail
*/
flt wolff_omp(Lattice &lattice, flt const& T, flt const& J, 
              flt const& MaxTime, u64 const& MaxSteps);


#endif