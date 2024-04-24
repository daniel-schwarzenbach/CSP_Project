#include "wolf.h++"

// Function to build the cluster for checking if neighbors have been visited or not, initialize with false for all (x,y,z)
vector<vector<vector<bool>>> checked(const uint Lx, const uint Ly, const uint Lz) {
    // Initialize the 3D vector representing the lattice
    vector<vector<vector<bool>>> visited(Lx, vector<vector<bool>>(Ly, vector<bool>(Lz)));
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
    flt cdot = 2*beta*(spin_r | spin_x)*(spin_r | spin_y);
    flt activate_prob;

    if (cdot > 0) {
        flt activate_prob =  0.0f;
    }
    else {
        flt activate_prob = 1-exp(cdot);
    }
    flt p = randflt();
    return (p <= activate_prob);
}

int wolf_algorithm(Lattice& lattice, flt beta){

    uint Lx = lattice.Lx();
    uint Ly = lattice.Ly();
    uint Lz = lattice.Lz();

    //Create vector that checks whether the site has been checked
    vector<vector<vector<bool>>> visited = checked(Lx, Ly, Lz);

    //Define stack for adding and removing lattice sites that are flipped, continue until it is empty (no new sites were added)
    std::vector<std::vector<uint>> stack;

    //Define cluster for adding the lattice sites that belong to the cluster (for computing average lattice size)
    std::vector<std::vector<uint>> cluster;

    // Choose random reflection
    Spin spin_r; 

    // Choose random lattice site as first point of cluster
    uint x = randflt()*Lx;
    uint y = randflt()*Ly;
    uint z = randflt()*Lz;
    
    //Define spin_x to be flipped, first point of the cluster
    Spin spin_x = lattice(x,y,z);

    //Flip sigma_x and mark x
    flip_spin(spin_r, spin_x);

    //Add spin_x to stack & cluster, mark site as checked
    stack.push_back({x,y,z});
    cluster.push_back({x,y,z});
    visited[x][y][z] = true;

    //Iterate over nearest neighbors until stack is empty, i.e. no newly adjoined sites
    while(!stack.empty()){
        
        const std::vector<uint>& current = stack.back();
        stack.pop_back();

        //Get current lattice position
        uint cx = current[0];
        uint cy = current[1];
        uint cz = current[2];

        //Mark as visited
        visited[cx][cy][cz] = true;

        for (uint i = -1; i <= 1; ++i){
            for (uint j = -1; j <= 1; ++j){
                for (uint k = -1; k <=1; ++k){
                    if (i==0 && j==0 && k==0) continue; //Skip original site

                    //Implement periodic boundary conditions
                    uint nx = (cx + i + Lx) % Lx;
                    uint ny = (cy + j + Ly) % Ly;
                    uint nz = (cz + k + Lz) % Lz;
                    
                    if (!visited[nx][ny][nz]){
                        Spin spin_y = lattice(nx,ny,nz); //Define spin sigma_y
                        visited[nx][ny][nz] = true; //Mark as visited

                        //If Bond is activated...
                        if (activate_bond(spin_x, spin_r, beta, spin_y)){
                            flip_spin(spin_r, spin_y); //...flip spin
                            stack.push_back({nx,ny,nz}); // ...add to stack 
                            cluster.push_back({nx,ny,nz}); // ...add to cluster (mark y)
                        }
                    }
                }
            }
        }
    }

    //Compute cluster size
    size_t clusterSize = cluster.size();

    if (clusterSize == 0 ){
        return -1;
    }

    return clusterSize;
}

//bool wolf(Lattice& lattice);
/*
performs the wolf algoritm on the lattice

- L: Lattice
- returns: succeptibility if it sucseeds
- can throw
*/

flt wolf(Lattice& lattice, flt T, flt Time){
    // to implement

    flt kB = 1.380649e-23f;
    flt beta = 1.0f / (T*kB);

    uint Nruns = 100;

    std::vector<uint> clusters;

    for (int i = 0; i <= Nruns; ++i){
        uint clusterSize = wolf_algorithm(lattice, beta);
        if(clusterSize == -1){
            return false;
            std::cout << "ERROR" << std::endl;
        }

        clusters.push_back(clusterSize);
    }

    uint totalClusterSize = 0;
    for (uint size : clusters) {
        totalClusterSize += size;
    }
    
    flt averageClusterSize = static_cast<flt>(totalClusterSize) / Nruns;
    
    flt susceptibility = averageClusterSize;

    return susceptibility;
}