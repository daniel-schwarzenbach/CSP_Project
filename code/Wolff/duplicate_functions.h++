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

bool activate_spin_spin(Spin& spin_x, Spin& spin_r, flt beta, Spin& spin_y);

void check_neighbor(Lattice& lattice, int nx, int ny, int nz, Spin& spin_x, Spin& spin_r, Array<Array<Array<bool>>>& visited, Array<Index>& stack, Array<Index>& cluster, flt beta);

#endif
