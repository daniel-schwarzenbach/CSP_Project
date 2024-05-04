#include <Heisenberg.h++>
using Index = StaticArray<int, 3>;

template <class T>
using Array3D = Array<Array<Array<T>>>;


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

bool activate_bond( Spin& spin_x, Spin& spin_r, flt beta, Spin& spin_y){
    flt cdot = 2 * beta * (spin_r | spin_x) * (spin_r | spin_y);
    F64 active = 1.0 - std::exp(min(F64(cdot), 0.0));
    flt p = rng::rand_f64();
    return (p <= active);
}

//Function to activate bonds between spins
// bool activate_spin_spin(Spin& spin_x, Spin& spin_r, flt beta, Spin& spin_y){
//     Spin spin_x_r = get_flipped_spin(spin_r,spin_x);
//     flt Z1 = spin_x_r | spin_y;
//     flt Z2 = spin_x | spin_y;
//     F64 active = min(0.0, 1 - std::exp(beta *( F64(Z1) - F64(Z2)) ));
//     flt p = rng::rand_f64();
//     return (p <= active);
// }

void check_neighbor(Lattice& lattice, int nx, int ny, int nz, Spin& spin_x, Spin& spin_r, Array<Array<Array<bool>>>& visited, Array<Index>& stack, Array<Index>& cluster, flt beta){
    Spin& spin_y = lattice(nx,ny,nz); //Define spin sigma_y (i.e. j)

    //If Bond is activated...
    if (activate_bond(spin_x, spin_r, beta, spin_y)){
        flip_spin(spin_r, spin_y); //...flip spin
        stack.push_back({nx,ny,nz}); // ...add to stack 
        cluster.push_back({nx,ny,nz}); // ...add to cluster (mark y)
        visited[nx][ny][nz] = true; //Mark as visited
    }
}

