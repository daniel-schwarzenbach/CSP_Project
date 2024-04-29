#include <Heisenberg.h++>


// Function to build the cluster for checking if neighbors have been 
// visited or not, initialize with false for all (x,y,z)
Array<Array<Array<bool>>> checked(const int Lx, const int Ly, const int Lz) {
    // Initialize the 3D vector representing the lattice
    Array<Array<Array<bool>>> visited(Lx,Array<Array<bool>>(Ly, Array<bool>(Lz)));

    // Assign false to all points in the checking_cluster
    for (int i = 0; i < Lx; ++i) {
        for (int j = 0; j < Ly; ++j) {
            for (int k = 0; k < Lz; ++k) {
                visited[i][j][k] = false;
            }
        }
    }
    return visited;
}

//Function to flip the spin
void flip_spin(Spin& spin_r, Spin& spin_x){
    flt cdot = spin_x | spin_r;
    spin_x = spin_x - (2.0f * cdot)*spin_r;
}