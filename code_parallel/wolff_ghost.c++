#include <Wolff/wolff.h++>
#include <Measure/Timekeeper.h++>
#include <Heisenberg.h++>

using Index = StaticArray<int, 3>;

template <class T>
using Array3D = Array<Array<Array<T>>>;



// Function to build the cluster for checking if neighbors have been 
// visited or not, initialize with false for all (x,y,z)
Array3D<bool> checked(const int Lx, const int Ly, const int Lz) {
    // Initialize the 3D vector representing the lattice
    Array3D<bool> visited(Lx,Array<Array<bool>>(Ly, Array<bool>(Lz)));
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
Vector3 flip_spin(Spin const& spin_r, Spin const& spin_x){
    flt cdot = spin_x | spin_r;
    return spin_x - (2.0f * cdot)*spin_r;
}

Matrix3x3 flip_ghost(Matrix3x3& ghost, Spin& spin_r){
    Matrix3x3 S = Matrix3x3::Identity() - 2*spin_r*spin_r.transpose(); //Build Householder matrix corresponding to the reflection wrt r
    return S * ghost * S; // Transform ghost cell 
}


//Function to activate bonds between spins
bool activate_spin_spin(Spin& spin_x, Spin& spin_r,
                        F64 beta,Spin& spin_y){
    Spin spin_x_r = flip_spin(spin_r,spin_x);
    flt Z1 = spin_x_r | spin_y;
    flt Z2 = spin_x | spin_y;
    F64 active = min(0.0, 1 - std::exp(beta *(F64(Z1)-F64(Z2)) ));
    F64 p = rng::rand_f64();
    return (p <= active);
}

//Function to activate bond between current ghost cell and neighboring spins
bool activate_ghost_spin(Spin& spin_x, Spin& spin_r, flt beta, Matrix3x3& ghost, Spin& H){
    
    Matrix ghost_inverse = ghost.inverse();
    Matrix ghost_reflected = flip_ghost(ghost,spin_r);
    Matrix ghost_reflected_inverse = ghost_reflected.inverse(); //Reflect and invert ghost matrix
    
    flt B1 = H | (ghost_reflected_inverse * spin_x); //B(s) = H^T * s = H^T * (SRS)^-1 * sigma_y
    flt B2 = H | (ghost_inverse * spin_x);
    
    F64 active = min(0.0, 1 - std::exp(beta *( F64(B1) - F64(B2)) ));
    flt p = rng::rand_f64();
    return (p <= active);
}

//Function to activate bond between current spin and its ghost cell
bool activate_spin_ghost(Spin& spin_x, Spin& spin_r, flt beta, Matrix& ghost, Spin& H){
    
    Matrix ghost_inverse = ghost.inverse(); //Invert matrix
    Spin spin_x_r = flip_spin(spin_r,spin_x); //Reflect spin
    
    flt B1 = H | (ghost_inverse * spin_x_r); //B(s) = H^T * s = H^T * (R)^-1 * (r*sigma_x)
    flt B2 = H | (ghost_inverse * spin_x); //B(s) = H^T * s = H^T * (R)^-1 * (sigma_x)
    
    F64 active = min(0.0, 1 - std::exp(beta *( F64(B1) - F64(B2)) ));
    flt p = rng::rand_f64();
    return (p <= active);
}


//If ghost cell is activated, check all lattice sites
void check_ghost(Lattice& lattice, Matrix& ghost, Spin& H, Spin& spin_r){
    uint Lx = lattice.Lx(); uint Ly = lattice.Ly(); 
    uint Lz = lattice.Lz();
    for(int i = 0; i <= Lx; ++i){
        for(int j=1; j <= Ly; ++j){
            for(int k=1; k <= Lz; ++k){
                //if(visited[i][j][k]==true) continue;

                Spin& spin_y = lattice(i,j,k);

                // if(activate_ghost_spin(spin_y)){
                //     flip_spin_ghost(spin_r, spin_y, H); //...flip spin
                    // stack.push_back({i,j,k}); // ...add to stack 
                    // cluster.push_back({i,j,k}); // ...add to cluster (mark y)
                }

            }
        }
    }
}



int wolf_algorithm(Lattice& lattice, flt beta){

    int Lx = lattice.Lx();
    int Ly = lattice.Ly();
    int Lz = lattice.Lz();

    //Create vector that checks whether the site has been checked
    Array<Array<Array<bool>>> visited = checked(Lx, Ly, Lz);

    //Define stack for adding and removing lattice sites that are flipped, continue until it is empty (no new sites were added)
    Array<Index> stack(0);

    //Define cluster for adding the lattice sites that belong to the cluster (for computing average lattice size)
    Array<Index> cluster(0);

    // Choose random reflection
    Spin spin_r = Spin::get_random();

    // Choose random lattice site as first point of cluster
    int x = rng::rand_f64()*Lx;
    int y = rng::rand_f64()*Ly;
    int z = rng::rand_f64()*Lz;
    
    //Define spin_x to be flipped, first point of the cluster
    Spin& spin_x = lattice(x,y,z);

    //Flip sigma_x and mark x
    flip_spin(spin_r, spin_x);

    //Add spin_x to stack & cluster, mark site as checked
    //stack.push_back({x,y,z});
    stack.push_back({x,y,z});
    cluster.push_back({x,y,z});
    visited[x][y][z] = true;

    //Iterate over nearest neighbors until stack is empty, i.e. no newly adjoined sites
    while(!stack.empty()){
        
        Index current = stack.back();
        stack.pop_back();

        //Get current lattice position
        int cx = current[0];
        int cy = current[1];
        int cz = current[2];

        //Mark as visited
        visited[cx][cy][cz] = true;

        for (int i = -1; i <= 1; ++i){
            for (int j = -1; j <= 1; ++j){
                for (int k = -1; k <=1; ++k){
                    if (i==0 && j==0 && k==0) continue; //Skip original site

                    //Implement periodic boundary conditions
                    int nx = modulo((cx + i), Lx);
                    int ny = modulo((cy + j), Ly);
                    int nz = modulo((cz + k), Lz);
                    
                    if (!visited[nx][ny][nz]){
                        Spin& spin_y = lattice(nx,ny,nz); //Define spin sigma_y

                        //If Bond is activated...
                        if (activate_bond(spin_x, spin_r, beta, spin_y)){
                            flip_spin(spin_r, spin_y); //...flip spin
                            stack.push_back({nx,ny,nz}); // ...add to stack 
                            cluster.push_back({nx,ny,nz}); // ...add to cluster (mark y)
                            visited[nx][ny][nz] = true; //Mark as visited
                        }
                    }
                }
            }
        }

        if(activate_spin_ghost(spin_x, spin_r, beta, ghost, H)){
            flip_ghost(ghost, spin_r);
            check_ghost(lattice, ghost, H, spin_r);
        }
    }

    //Compute cluster size
    size_t clusterSize = cluster.size();

    return clusterSize;
}

//bool wolff(Lattice& lattice);
/*
performs the wolff algoritm on the lattice

- L: Lattice
- returns: succeptibility if it sucseeds
- can throw
*/

F64 wolff_ghost(Lattice& lattice, F64 T, F64 J, F64 MaxTime, uint MaxSteps){

    F64 beta = Beta(T);

    TimeKeeper watch;

    Array<int> clusters;
    uint nRuns = 0;

    for (uint i = 0; i <= MaxSteps; ++i){
        uint clusterSize = wolf_algorithm(lattice, beta);
        if(clusterSize == -1){
            return false;
            std::cout << "ERROR" << std::endl;
        }
        clusters.push_back(clusterSize);

        ++nRuns;
        // Check if maximum time has been reached
        if (watch.time() >= MaxTime) {
            break; // Stop simulation if maximum time reached
        }
    }

    uint totalClusterSize = 0;
    for (int size : clusters) {
        totalClusterSize += size;
    }
    
    F64 averageClusterSize = static_cast<F64>(totalClusterSize)/nRuns;
    
    F64 susceptibility = averageClusterSize;

    return susceptibility;
}

