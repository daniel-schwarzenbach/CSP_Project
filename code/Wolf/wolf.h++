#ifndef WOLF_HPP
#define WOLF_HPP

#include <Heisenberg.h++>
#include <algorithm>
#include <cmath>

using flt = float;
using namespace std;

/*
wolf algorithm for the Heisenberg3D model

/ @brief
/ @param lattice our 3d lattice, where to perform the simulation on
/ @return if the procedure succeds
/ @exception may fail
*/

// Function to build the cluster for checking if neighbors have been marked or not, initialize with 0 for all (x,y,z)
vector<vector<vector<uint>>> checked(const uint Lx, const uint Ly, const uint Lz) {
    // Initialize the 3D vector representing the lattice
    vector<vector<vector<int>>> checkCluster(Lx, vector<vector<int>>(Ly, vector<int>(Lz)));

    // Assign zero to all points in the checking_cluster
    for (int i = 0; i < Lx; ++i) {
        for (int j = 0; j < Ly; ++j) {
            for (int k = 0; k < Lz; ++k) {
                checkCluster[i][j][k] = 0;
            }
        }
    }
    return checkCluster;
}

//Function to flip the spin
void flip_spin(const Spin& spin_r, Spin& spin_x){
    spin_x = spin_x - 2*(spin_x | spin_r)*spin_r;
}


bool activate_bond( Spin& spin_x, Spin& spin_r, flt beta, const Spin& spin_y){
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

void add_spin_to_cluster(std::vector<Spin>& cluster, const Spin& spin) {
    cluster.push_back(spin);
}

bool check_and_activate_link(const uint i, const uint j, const uint k, const Spin& spin_x, const Spin& spin_r, flt beta, Lattice& lattice, vector<vector<vector<int>>>& checkCluster, std::vector<Spin>& SpinCluster){
    if (checkCluster[i][j][k] == 0){
        Spin spin_y = lattice(i,j,k);
        if (activate_bond(spin_x, spin_r, beta, spin_y) == true){
            flip_spin(spin_r, spin_y);
            add_spin_to_cluster(SpinCluster, spin_y);
            checkCluster[i][j][k] = 2;
        }
        else {
            checkCluster[i][j][k] = 1;
        }
    }
    else {
        std::cout << "This link has been checked already" << std::endl; 
        return false;
    }
    return true;
}

bool check_nearest_neighbors(const uint i, const uint j, const uint k, const Spin& spin_x, const Spin& spin_r, flt beta, Lattice& lattice, vector<vector<vector<int>>>& checkCluster, std::vector<Spin>& SpinCluster){
    bool front = check_and_activate_link(i+1,j,k, spin_x, spin_r, beta, lattice, checkCluster, SpinCluster);
    bool back = check_and_activate_link(i-1,j,k, spin_x, spin_r, beta, lattice, checkCluster, SpinCluster);
    bool right = check_and_activate_link(i,j+1,k, spin_x, spin_r, beta, lattice, checkCluster, SpinCluster);
    bool left = check_and_activate_link(i,j-1,k, spin_x, spin_r, beta, lattice, checkCluster, SpinCluster);
    bool up = check_and_activate_link(i,j,k+1, spin_x, spin_r, beta, lattice, checkCluster, SpinCluster);
    bool down = check_and_activate_link(i,j,k-1, spin_x, spin_r, beta, lattice, checkCluster, SpinCluster);    
}

bool check_nearest_neighbors(const uint i, const uint j, const uint k, const Spin& spin_x, const Spin& spin_r, flt beta, Lattice& lattice, vector<vector<vector<int>>>& checkCluster, std::vector<Spin>& SpinCluster){
    /* if neighbor is unchecked -> check -> activate
    Visit all links connecting x to its nearest neighbor y. Activate bond with probability 
    P(sigma_x, sigma_y) = 1 - exp{min[0, 2beta(r\cdot sigma_x)(r\cdot sigma_y)]}
    If activated flip sigma_y and mark y
    */

    if (checkCluster[i+1][j][k] == 0){
        Spin spin_y = lattice(i+1,j,k);
        if (activate_bond(spin_x, spin_r, beta, spin_y) == true){
            flip_spin(spin_r, spin_y);
            add_spin_to_cluster(SpinCluster, spin_y);
            checkCluster[i+1][j][k] = 2;
        }
        else {
            checkCluster[i+1][j][k] = 1;
        }
    }

    if (checkCluster[i-1][j][k] == 0){
        Spin spin_y = lattice(i-1,j,k);
        if (activate_bond(spin_x, spin_r, beta, spin_y) == true){
            flip_spin(spin_r, spin_y);
            add_spin_to_cluster(SpinCluster, spin_y);
            checkCluster[i-1][j][k] = 2;
        }
        else {
            checkCluster[i-1][j][k] = 1;
        }
    }

    if (checkCluster[i][j+1][k] == 0){
        Spin spin_y = lattice(i,j+1,k);
        if (activate_bond(spin_x, spin_r, beta, spin_y) == true){
            flip_spin(spin_r, spin_y);
            add_spin_to_cluster(SpinCluster, spin_y);
            checkCluster[i][j+1][k] = 2;
        }
        else {
            checkCluster[i][j+1][k] = 1;
        }
    }
    if (checkCluster[i][j-1][k] == 0){
        Spin spin_y = lattice(i,j-1,k);
        if (activate_bond(spin_x, spin_r, beta, spin_y) == true){
            flip_spin(spin_r, spin_y);
            add_spin_to_cluster(SpinCluster, spin_y);
            checkCluster[i][j-1][k] = 2;
        }
        else {
            checkCluster[i][j-1][k] = 1;
        }
    }
    if (checkCluster[i][j][k+1] == 0){
        Spin spin_y = lattice(i,j,k+1);
        if (activate_bond(spin_x, spin_r, beta, spin_y) == true){
            flip_spin(spin_r, spin_y);
            add_spin_to_cluster(SpinCluster, spin_y);
            checkCluster[i][j][k+1] = 2;
        }
        else {
            checkCluster[i][j][k+1] = 1;
        }
    }
    if (checkCluster[i][j][k-1] == 0){
        Spin spin_y = lattice(i,j,k-1);
        if (activate_bond(spin_x, spin_r, beta, spin_y) == true){
            flip_spin(spin_r, spin_y);
            add_spin_to_cluster(SpinCluster, spin_y);
            checkCluster[i][j][k-1] = 2;
        }
        else {
            checkCluster[i][j][k-1] = 1;
        }
    }

    else{
        std::cout << "All neighbors have been checked" << std::endl;
        return false;
    }

    return true;

}


Lattice wolf_algorithm(Lattice& lattice){
    flt T = 30.0f;
    flt kB = 1.380649e-23f;
    flt beta = 1.0f / (T*kB);
    
    /*Define checking matrix
    Unchecked sites have value 0
    Checked sites that were not flipped have value 1 (s.t. they are only checked once)
    Checked sites that were flipped have value 2
    */

    uint Lx = lattice.Lx();
    uint Ly = lattice.Ly();
    uint Lz = lattice.Lz();

    //Create vector that checks whether the site has been checked
    vector<vector<vector<uint>>> visited = checked(Lx, Ly, Lz);

    //Define stack for adding and removing lattice sites that are flipped, continue until it is empty (no new sites were added)
    std::vector<std::vector<std::vector<uint>>> stack;

    //Define cluster for adding the lattice sites that belong to the cluster (for computing average lattice size)
    std::vector<std::vector<std::vector<uint>>> cluster;

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
    visited[x][y][z] = 1;

    //Iterate over nearest neighbors until stack is empty, i.e. no newly adjoined sites
    while(!stack.empty()){

        for (i = -1; i <= 1, ++i){
            for (j = -1; j <= 1; ++j){
                for (k = -1; k <=1; ++k){
                    if (i==0 & j==0 & k==0) continue; //Skip original site
                    


                }
            }

        }


    }


    //Check neighbors of x, activate the bond and add spin to cluster.

    //Compute mean cluster size

}

bool wolf(Lattice& lattice);

#endif