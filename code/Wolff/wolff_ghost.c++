#include <Wolff/wolff_ghost.h++>
#include <Measure/Timekeeper.h++>
#include <Wolff/duplicate_functions.h++>

using Index = StaticArray<int, 3>;

template <class T>
using Array3D = Array<Array<Array<T>>>;


//Generate ghost cell i.e. a random matrix in O(3)
Matrix3x3 generate_ghost() {
    Eigen::Quaternionf randomQuaternion = Eigen::Quaternionf::UnitRandom();
    Matrix3x3 randomOrthogonalMatrix = randomQuaternion.normalized().toRotationMatrix();
    return randomOrthogonalMatrix;
}

//Returns the flipped spin given a random spin r, w.r.t the hyperplane orthogonal to r
Spin get_flipped_spin(Spin& spin_r, Spin& spin_x){
    flt cdot = spin_x | spin_r;
    Spin spin_x_r = Spin::get_random();
    spin_x_r = spin_x - (2.0f * cdot)*spin_r;
    return spin_x_r;
}

//Flip ghost cell R as R' = SRS 
void flip_ghost(Matrix3x3& ghost, Spin& spin_r){
    Matrix3x3 I = Matrix3x3::Identity(3,3);
    Vector3 R = spin_r;
    Matrix3x3 S = I - 2 * R * R.transpose(); //Build Householder matrix corresponding to the reflection wrt r
    ghost = S * ghost; // Transform ghost cell 
}

//Return flipped ghost cell
Matrix3x3 get_flipped_ghost(Matrix3x3& ghost, Spin& spin_r){
    Matrix3x3 I = Matrix3x3::Identity(3,3);
    Vector3 R = spin_r;
    Matrix3x3 S = I - 2 * R * R.transpose(); //Build Householder matrix corresponding to the reflection wrt r
    ghost = S * ghost ; // Transform ghost cell 
    return ghost;
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

//Function to activate bond depending on given probability 


//Function to activate bond between current ghost cell and neighboring spins
bool activate_ghost_spin(Spin& spin_x, Spin& spin_r, flt beta, Matrix3x3& ghost, Spin& H){
    
    Matrix3x3 ghost_inverse = ghost.inverse();
    Matrix3x3 ghost_reflected = get_flipped_ghost(ghost,spin_r);
    Matrix3x3 ghost_reflected_inverse = ghost_reflected.inverse(); //Reflect and invert ghost matrix
    Vector3 spin = spin_x;
    Vector3 mag_field = H;
    
    flt B1 = mag_field.dot(ghost_reflected_inverse * spin); //B(s) = H^T * s = H^T * (SRS)^-1 * sigma_y
    flt B2 = mag_field.dot(ghost_inverse * spin);
    
    F64 active = min(0.0, 1 - std::exp(beta *( F64(B1) - F64(B2))));
    flt p = rng::rand_f64();
    return (p <= active);
}

//Function to activate bond between current spin and its ghost cell
//Work with matrices everywhere s.t. we can use Eigen operations
bool activate_spin_ghost(Spin& spin_x, Spin& spin_r, flt beta, Matrix3x3& ghost, Spin& H){
    
    Matrix3x3 ghost_inverse = ghost.inverse(); //Invert matrix
    Vector3 spin_x_r = get_flipped_spin(spin_r,spin_x); //Reflect spin
    Vector3 mag_field = H;
    F64 B1 = mag_field.dot(ghost_inverse * spin_x_r); //B(s) = H^T * s = H^T * (R)^-1 * (r*sigma_x)
    F64 B2 = mag_field.dot(ghost_inverse * spin_x); //B(s) = H^T * s = H^T * (R)^-1 * (sigma_x)
    
    F64 active = min(0.0, 1 - std::exp(beta *( B1 - B2) ));
    flt p = rng::rand_f64();
    return (p <= active);
}


//If ghost cell is activated, check all lattice sites
void check_ghost(Lattice& lattice, int Lx, int Ly,int Lz, Matrix3x3& ghost, Spin& H, Spin& spin_r, flt beta, Array<Index> stack, Array<Index> cluster, Array<Array<Array<bool>>> visited){
    for(int i = 0; i <= Lx; ++i){
        for(int j = 1; j <= Ly; ++j){
            for(int k = 1; k <= Lz; ++k){
                if(visited[i][j][k] == true) continue; 
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



int wolff_ghost_algorithm(Lattice& lattice, flt beta){

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
    Matrix3x3 ghost = generate_ghost();

    // Choose random reflection
    Spin spin_r = Spin::get_random();
    //Spin H = Spin::get_random(); //Initialize random external magnetic field
    Spin H = Spin(0.0,0.0,10000000.0);

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

        Spin& spin_x = lattice(cx,cy,cz);

        if(!visited[(cx+1+Lx)%Lx][cy][cz]){ check_neighbor(lattice, (cx+1+Lx)%Lx, cy, cz, spin_x, spin_r, visited, stack, cluster, beta); }
        if(!visited[(cx-1+Lx)%Lx][cy][cz]){ check_neighbor(lattice, (cx-1+Lx)%Lx, cy, cz, spin_x, spin_r, visited, stack, cluster, beta); }
        if(!visited[cx][(cx+1+Lx)%Ly][cz]){ check_neighbor(lattice, cx, (cx+1+Lx)%Ly, cz, spin_x, spin_r, visited, stack, cluster, beta); }
        if(!visited[cx][(cx-1+Lx)%Ly][cz]){ check_neighbor(lattice, cx, (cx-1+Lx)%Ly, cz, spin_x, spin_r, visited, stack, cluster, beta); }
        if(!visited[cx][cy][(cz+1+Lx)%Lz]){ check_neighbor(lattice, cx, cy, (cz+1+Lx)%Lz, spin_x, spin_r, visited, stack, cluster, beta); }
        if(!visited[cx][cy][(cz-1+Lx)%Lz]){ check_neighbor(lattice, cx, cy, (cz-1+Lx)%Lz, spin_x, spin_r, visited, stack, cluster, beta); }

        Spin& spin_current = lattice(cx,cy,cz);

        //Check if bond to ghost cell is activated...
        if(activate_spin_ghost(spin_current, spin_r, beta, ghost, H)){
            check_ghost(lattice, Lx, Ly, Lz, ghost, H, spin_r, beta, stack, cluster, visited); //immediately check all lattice sites s.t. we do not need to store ghost cell in stack
            flip_ghost(ghost, spin_r); // flip trafo
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
        uint clusterSize = wolff_ghost_algorithm(lattice, beta);

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