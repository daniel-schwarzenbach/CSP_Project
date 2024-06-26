#ifndef __WOLFF_H__
#define __WOLFF_H__

#include <Heisenberg.h++>
#include <algorithm>
#include <cmath>
#include <algorithm>

/*
wolff algorithm for the Heisenberg 3D model

/ @param Lattice3D<Spin>: our 3d Lattice3D<Spin>, where to perform the simulation on
/ @param T: temperature
/ @param J: spin interaction strenth
/ @param maxTimeSeconds: the maximal time the simulation will run
/ @param maxSteps: the maximal number of steps the simulation will run
/ @return average cluster size
*/
flt wolff(Lattice3D<Spin> &lattice, flt const &T, flt const &J,
          flt const &MaxTime, u64 const &MaxSteps);


/*
wolff algorithm for the Heisenberg 3D model parrallel

/ @param Lattice3D<Spin>: our 3d Lattice3D<Spin>, where to perform the simulation on
/ @param T: temperature
/ @param J: spin interaction strenth
/ @param maxTimeSeconds: the maximal time the simulation will run
/ @param maxSteps: the maximal number of steps the simulation will run
/ @return average cluster size
*/
flt wolff_omp(Lattice3D<Spin> &lattice, flt const &T, flt const &J,
          flt const &MaxTime, u64 const &MaxSteps);


#endif // __WOLFF_H__