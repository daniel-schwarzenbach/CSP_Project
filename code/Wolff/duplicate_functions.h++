#ifndef DUPLICATE_WOLFF
#define DUPLICATE_WOLFF

#include <Heisenberg.h++>
#include <algorithm>
#include <cmath>

using Index = StaticArray<int, 3>;

template <class T>
using Array3D = Array<Array<Array<T>>>;

// Function to build the cluster for checking if neighbors have been 
// visited or not, initialize with false for all (x,y,z)
Array3D<bool> checked(const int Lx, const int Ly, const int Lz);

//Function to flip the spin
void flip_spin(Spin& spin_r, Spin& spin_x);

#endif
