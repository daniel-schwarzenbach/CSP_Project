#include <Wolff/wolff_ghost.h++>
#include <Measure/Timekeeper.h++>
#include <Wolff/duplicate_functions.h++>
#include <Heisenberg.h++>
#include <unordered_set>

using Index = std::array<int, 3>;

template <class T>
using Array3D = Array<Array<Array<T>>>;


// Generate ghost cell i.e. a random matrix in O(3)
Matrix3x3 generate_ghost() {
    Eigen::Quaternionf randomQuaternion = Eigen::Quaternionf::UnitRandom();
    Matrix3x3 randomOrthogonalMatrix = randomQuaternion.toRotationMatrix();
    flt determinant = randomOrthogonalMatrix.determinant();
    Eigen::Matrix<float, 3, 3, 0, 3, 3> matrix = randomOrthogonalMatrix;
    Matrix3x3 id = matrix * matrix.transpose();
    flt a = (matrix * matrix.transpose() - Matrix3x3::Identity(3,3)).norm() < 1e-10;
    bool test2 = std::abs(matrix.determinant() - 1.0f) < 0.01f;

    return randomOrthogonalMatrix;
}

bool is_orthogonal(Matrix3x3 matrix){
    flt a = (matrix * matrix.transpose() - Matrix3x3::Identity(3,3)).norm() < 1e-10;
    bool test1 = std::abs(matrix.determinant() - 1.0f) < 0.0001f;
    bool test2 = std::abs(matrix.determinant() + 1.0f) < 0.0001f;
    return (test1 || test2);

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
    bool a = is_orthogonal(S*ghost);
    ghost = S * ghost; // Transform ghost cell 
    
    
}

//Return flipped ghost cell
Matrix3x3 get_flipped_ghost(Matrix3x3& ghost, Spin& spin_r){
    Matrix3x3 I = Matrix3x3::Identity(3,3);
    Vector3 R = spin_r;
    Matrix3x3 S = I - 2 * R * R.transpose(); //Build Householder matrix corresponding to the reflection wrt r
    ghost = S * ghost ; // Transform ghost cell 
    bool a = is_orthogonal(S*ghost);
    return ghost;
}
    
//Function to activate bonds between spins
bool activate_spin_spin(Spin& spin_x, Spin& spin_x_r, Spin& spin_r, flt beta, Spin& spin_y){
    //Spin spin_x_r = get_flipped_spin(spin_r,spin_x);
    flt Z1 = spin_x_r | spin_y;
    flt Z2 = spin_x | spin_y;
    F64 dE = Z2 - Z1;
    F64 active;
    if (dE < 0) {
                active = 0;
              } else {
                active = 1.0 - exp(-dE * beta);
              }
    //F64 active = 1 - exp(std::min(0.0, (beta *( F64(B1) - F64(B2)) )));
    flt p = rng::rand_f64();
    return (p <= active);
}

//Function to activate bond depending on given probability 


//Function to activate bond between current ghost cell and neighboring spins
bool activate_ghost_spin(Spin& spin_x, Spin& spin_r, flt beta, Matrix3x3& ghost, Spin& H){
    
    Matrix3x3 ghost_inverse = ghost.inverse();
    Matrix3x3 ghost_reflected = get_flipped_ghost(ghost,spin_r);
    Matrix3x3 ghost_reflected_inverse = ghost_reflected.inverse(); //Reflect and invert ghost matrix
    
    bool a = is_orthogonal(ghost_inverse);
    bool b = is_orthogonal(ghost_reflected);
    bool c = is_orthogonal(ghost_reflected_inverse);
    Vector3 spin = spin_x;
    Vector3 mag_field = H;
    
    flt B1 = mag_field.dot(ghost_reflected_inverse * spin); //B(s) = H^T * s = H^T * (SRS)^-1 * sigma_y
    flt B2 = mag_field.dot(ghost_inverse * spin);

    F64 dE = B2 - B1;
    F64 active;
    if (dE < 0) {
                active = 0;
              } 
    else {
                active = 1.0 - exp(-dE * beta);
              }
    //F64 active = 1 - exp(std::min(0.0, (beta *( F64(B1) - F64(B2)) )));
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
    F64 dE = B2 - B1;
    F64 active;
    if (dE < 0) {
                active = 0;
              } else {
                active = 1.0 - exp(-dE * beta);
              }
    //F64 active = 1 - exp(std::min(0.0, (beta *( F64(B1) - F64(B2)) )));
    flt p = rng::rand_f64();
    return (p <= active);
}


//If ghost cell is activated, check all lattice sites that haven't been already checked
void check_ghost(int b, Lattice& lattice, int Lx, int Ly,int Lz, Matrix3x3& ghost, Spin& H, Spin& spin_r, flt beta, Array<Index>& stack, std::unordered_set<Index, IndexHash>& inCluster, Array<Array<Array<bool>>>& visited){
    for(int i = 0; i < Lx; ++i){
        for(int j = 0; j < Ly; ++j){
            for(int k = 0; k < Lz; ++k){
                if(visited[i][j][k] == true) continue; //Don't check one that has been checked already
                //If this is true it means that the site has been added already and 
                //so the bond to the ghost cell was checked at that point

                Spin& spin_y = lattice(i,j,k); //get spin

                //Function to activate bond between current ghost cell and neighboring spins
                if(activate_ghost_spin(spin_y, spin_r, beta, ghost, H)){
                    b += 1;
                    //flip_spin(spin_r, spin_y); // ...flip spin
                    stack.push_back({i,j,k}); // ...add to stack 
                    inCluster.insert({i,j,k}); // ...add to cluster (mark y)
                    //visited[i][j][k] = true; // Mark as visited
                }

            }
        }

    }
}

void check_neighbor_ghost(Lattice& lattice, int nx, int ny, int nz, Spin& spin_x, Spin& spin_x_r, Spin& spin_r, Array<Array<Array<bool>>>& visited, Array<Index>& stack, std::unordered_set<Index, IndexHash>& inCluster, flt beta){
    if(!visited[nx][ny][nz]) {
        
        Spin spin_y = lattice(nx,ny,nz); //Define spin sigma_y (i.e. j)
        //If Bond is activated...
        if (activate_spin_spin(spin_x, spin_x_r, spin_r, beta, spin_y)){
            //flip_spin(spin_r, spin_y); //...flip spin
            stack.push_back({nx,ny,nz}); // ...add to stack 
            inCluster.insert({nx,ny,nz}); // ...add to cluster (mark y)
            //visited[nx][ny][nz] = true; //Mark as visited
        }
    }
}


int wolff_ghost_algorithm(Lattice& lattice, flt beta, Spin H, Matrix3x3& ghost){

    int Lx = lattice.Lx();
    int Ly = lattice.Ly();
    int Lz = lattice.Lz();

    //Create vector that checks whether the site has been checked
    Array<Array<Array<bool>>> visited = checked(Lx, Ly, Lz);

    //Define stack for adding and removing lattice sites that are flipped, continue until it is empty (no new sites were added)
    Array<Index> stack(0);
    stack.reserve(Lx*Ly*Lz); //Make sure we have enough space to create a cluster with all lattice sizes

    //Define cluster for adding the lattice sites that belong to the cluster (for computing average lattice size)
    Array<Index> cluster(0);
    cluster.reserve(Lx*Ly*Lz); //Make sure we have enough space to create a cluster with all lattice sizes
    
    std::unordered_set<Index, IndexHash> inCluster; //
    inCluster.reserve(Lx*Ly*Lz);


    //Define ghost cell which contains a transformation from O(3)
    
    // Choose random reflection
    Spin spin_r = Spin::get_random();
    //Spin spin_r = Spin(0.0,0.0,1.0);
    //Spin H = Spin::get_random(); //Initialize random external magnetic field

    // Choose random lattice site as first point of cluster
    int x = rng::rand_f64()*(Lx-1);
    int y = rng::rand_f64()*(Ly-1);
    int z = rng::rand_f64()*(Lz-1);
    
    //Define spin_x to be flipped, first point of the cluster (random site m_0)
    Spin& spin_x= lattice(x,y,z);

    //Add spin_x to stack & cluster, mark site as checked
    //stack.push_back({x,y,z});
    stack.push_back({x,y,z});
    //cluster.push_back({x,y,z});
    inCluster.insert({x,y,z});
    //visited[x][y][z] = true;

    //Flip sigma_x and mark x
    //flip_spin(spin_r, spin_x);

    int b = 0;

    //Iterate over nearest neighbors until stack is empty, i.e. no newly adjoined sites
    while(!stack.empty()){
        
        Index current = stack.back();
        stack.pop_back();

        //Get current lattice position
        int cx = current[0];
        int cy = current[1];
        int cz = current[2];

        if(visited[cx][cy][cz] == false){
            //Mark as visited
            visited[cx][cy][cz] = true;
        
            Spin& spin_x = lattice(cx,cy,cz); //spin_x = m
            Spin spin_x_r;
            spin_x_r = get_flipped_spin(spin_r, spin_x);

            //Check neighbors and add them to the stack if the bonds are activated. Add them to the cluster only if they are not there already
            check_neighbor_ghost(lattice, (cx+1+Lx) % Lx, cy, cz, spin_x, spin_x_r, spin_r, visited, stack, inCluster, beta);
            check_neighbor_ghost(lattice, (cx-1+Lx) % Lx, cy, cz, spin_x, spin_x_r, spin_r, visited, stack, inCluster, beta);
            check_neighbor_ghost(lattice, cx, (cy+1+Ly) % Ly, cz, spin_x, spin_x_r, spin_r, visited, stack, inCluster, beta);
            check_neighbor_ghost(lattice, cx, (cy-1+Ly) % Ly, cz, spin_x, spin_x_r, spin_r, visited, stack, inCluster, beta);
            check_neighbor_ghost(lattice, cx, cy, (cz+1+Lz) % Lz, spin_x, spin_x_r, spin_r, visited, stack, inCluster, beta);
            check_neighbor_ghost(lattice, cx, cy, (cz-1+Lz) % Lz, spin_x, spin_x_r, spin_r, visited, stack, inCluster, beta);

            if(H != Spin(0.0,0.0,0.0)) {
                //Check if bond to ghost cell is activated...
                if(activate_spin_ghost(spin_x, spin_r, beta, ghost, H)){
                    check_ghost(b, lattice, Lx, Ly, Lz, ghost, H, spin_r, beta, stack, inCluster, visited); //immediately check all lattice sites s.t. we do not need to store ghost cell in stack
                    flip_ghost(ghost, spin_r); // flip trafo
                }
                //cout << "Magnetic field check" << endl;
            }

            flip_spin(spin_r, spin_x);
        }
    }

    //Compute cluster size
    size_t clusterSize = inCluster.size();

    return clusterSize;

}

//bool wolff(Lattice& lattice);
/*
performs the wolff algoritm on the lattice

- L: Lattice
- returns: succeptibility if it sucseeds
- can throw
*/

F64 wolff_ghost(Lattice& lattice, F64 T, F64 J, F64 MaxTime, uint MaxSteps, Spin h){

    F64 beta = Beta(T);

    TimeKeeper watch;

    Array<int> clusters;
    uint nRuns = 0;

    

    for (uint i = 0; i <= MaxSteps; ++i){
        Matrix3x3 ghost = generate_ghost();

        uint clusterSize = wolff_ghost_algorithm(lattice, beta, h, ghost);

        clusters.push_back(clusterSize);

        ++nRuns;
        // Check if maximum time has been reached
        if (watch.time() >= MaxTime) {
            cout << "Run stops due to timeout" << endl;
            break; // Stop simulation if maximum time reached
        }
    }

    uint totalClusterSize = 0;
    for (int size : clusters) {
        totalClusterSize += size; 
    }
    
    F64 averageClusterSize = static_cast<F64>(totalClusterSize)/nRuns;

    return averageClusterSize;
}