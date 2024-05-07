#ifndef __WOLFF_H__
#define __WOLFF_H__

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
flt wolff(Lattice3D<Spin> &lattice, flt const &T, flt const &J,
          flt const &MaxTime, u64 const &MaxSteps, Spin const &h);


#endif // __WOLFF_H__