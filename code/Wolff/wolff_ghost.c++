#include <Wolff/wolff.h++>
#include <Measure/Timekeeper.h++>

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

MatrixNxM generate_ghost() {
    Eigen::Quaternionf randomQuaternion = Eigen::Quaternionf::UnitRandom();
    MatrixNxM randomOrthogonalMatrix = randomQuaternion.normalized().toRotationMatrix();
    return randomOrthogonalMatrix;
}

//Function to flip the spin
void flip_spin(Spin& spin_r, Spin& spin_x){
    flt cdot = spin_x | spin_r;
    spin_x = spin_x - (2.0f * cdot)*spin_r;
}

Spin get_flipped_spin(Spin& spin_r, Spin& spin_x){
    flt cdot = spin_x | spin_r;
    spin_x = spin_x - (2.0f * cdot)*spin_r;
    return spin_x;
}

void flip_ghost(MatrixNxM& ghost, Spin& spin_r){
    MatrixNxM S = MatrixNxM::Identity(3,3);
   // MatrixNxM S = I - 2*spin_r*spin_r;//spin_r_conj_transpose; //Build Householder matrix corresponding to the reflection wrt r
    ghost = S * ghost * S; // Transform ghost cell 
}

MatrixNxM get_flipped_ghost(MatrixNxM& ghost, Spin& spin_r){
    MatrixNxM S = MatrixNxM::Identity(3,3);
    //MatrixNxM S = I - 2*spin_r*spin_r;//spin_r_conj_transpose; //Build Householder matrix corresponding to the reflection wrt r
    ghost = S * ghost * S; // Transform ghost cell 
    return ghost;
}
    
//Function to activate bonds between spins
bool activate_spin_spin(Spin& spin_x, Spin& spin_r, flt beta, Spin& spin_y){
    Spin spin_x_r = get_flipped_spin(spin_r,spin_x);
    flt Z1 = spin_x_r | spin_y;
    flt Z2 = spin_x | spin_y;
    F64 active = min(0.0, 1 - std::exp(beta *( F64(Z1) - F64(Z2)) ));
    flt p = rng::randflt();
    return (p <= active);
}

//Function to activate bond between current ghost cell and neighboring spins
bool activate_ghost_spin(Spin& spin_x, Spin& spin_r, flt beta, MatrixNxM& ghost, Spin& H){
    
    MatrixNxM ghost_inverse = ghost.inverse();
    MatrixNxM ghost_reflected = get_flipped_ghost(ghost,spin_r);
    MatrixNxM ghost_reflected_inverse = ghost_reflected.inverse(); //Reflect and invert ghost matrix
    
    flt B1 = H | (ghost_reflected_inverse * spin_x); //B(s) = H^T * s = H^T * (SRS)^-1 * sigma_y
    flt B2 = H | (ghost_inverse * spin_x);
    
    F64 active = min(0.0, 1 - std::exp(beta *( F64(B1) - F64(B2))));
    flt p = rng::randflt();
    return (p <= active);
}

//Function to activate bond between current spin and its ghost cell
bool activate_spin_ghost(Spin& spin_x, Spin& spin_r, flt beta, MatrixNxM& ghost, Spin& H){
    
    MatrixNxM ghost_inverse = ghost.inverse(); //Invert matrix
    Spin spin_x_r = get_flipped_spin(spin_r,spin_x); //Reflect spin
    
    flt B1 = H | (ghost_inverse * spin_x_r); //B(s) = H^T * s = H^T * (R)^-1 * (r*sigma_x)
    flt B2 = H | (ghost_inverse * spin_x); //B(s) = H^T * s = H^T * (R)^-1 * (sigma_x)
    
    F64 active = min(0.0, 1 - std::exp(beta *( F64(B1) - F64(B2)) ));
    flt p = rng::randflt();
    return (p <= active);
}


//If ghost cell is activated, check all lattice sites
void check_ghost(Lattice& lattice, int Lx, int Ly,int Lz, MatrixNxM& ghost, Spin& H, Spin& spin_r, flt beta, Array<Index> stack, Array<Index> cluster, Array<Array<Array<bool>>> visited){
    for(int i = 0; i <= Lx; ++i){
        for(int j=1; j <= Ly; ++j){
            for(int k=1; k <= Lz; ++k){
                if(visited[i][j][k]==true) continue; 
                //If this is true it means that the site has been added already and 
                //so the bond to the ghost cell was checked at that point

                Spin& spin_y = lattice(i,j,k); //get spin

                if(activate_ghost_spin(spin_y, spin_r, beta, ghost, H)){
                    flip_spin(spin_r, spin_y); // ...flip spin
                    stack.push_back({i,j,k}); // ...add to stack 
                    cluster.push_back({i,j,k}); // ...add to cluster (mark y)
                    visited[i][j][k] = true; // Mark as visited
                }

            }
        }
    }
}



int wolf_ghost_algorithm(Lattice& lattice, flt beta){

    int Lx = lattice.Lx();
    int Ly = lattice.Ly();
    int Lz = lattice.Lz();

    //Create vector that checks whether the site has been checked
    Array<Array<Array<bool>>> visited = checked(Lx, Ly, Lz);

    //Define stack for adding and removing lattice sites that are flipped, continue until it is empty (no new sites were added)
    Array<Index> stack(0);

    //Define cluster for adding the lattice sites that belong to the cluster (for computing average lattice size)
    Array<Index> cluster(0);

    //Define ghost cell which contains a transformation from O(3)
    MatrixNxM ghost = generate_ghost();

    // Choose random reflection
    Spin spin_r = Spin::get_random();
    Spin H = Spin::get_random(); //Initialize random external magnetic field

    // Choose random lattice site as first point of cluster
    int x = rng::randflt()*Lx;
    int y = rng::randflt()*Ly;
    int z = rng::randflt()*Lz;
    
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
                        if (activate_spin_spin(spin_x, spin_r, beta, spin_y)){
                            flip_spin(spin_r, spin_y); //...flip spin
                            stack.push_back({nx,ny,nz}); // ...add to stack 
                            cluster.push_back({nx,ny,nz}); // ...add to cluster (mark y)
                            visited[nx][ny][nz] = true; //Mark as visited
                        }
                    }
                }
            }
        }

        //Check if bond to ghost cell is activated...
        if(activate_spin_ghost(spin_x, spin_r, beta, ghost, H)){
            flip_ghost(ghost, spin_r); // flip trafo
            check_ghost(lattice, Lx, Ly, Lz, ghost, H, spin_r, beta, stack, cluster, visited); //immediately check all lattice sites s.t. we do not need to store ghost cell in stack
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
        uint clusterSize = wolf_ghost_algorithm(lattice, beta);
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

