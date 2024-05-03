#include <Wolff/wolff.h++>
#include <Measure/Timer.h++>
#include <Wolff/duplicate_functions.h++>

using Index = StaticArray<int, 3>;

template <class T>
using Array3D = Array<Array<Array<T>>>;


//Function to activate bond depending on given probability 
// bool activate_bond( Spin& spin_x, Spin& spin_r, flt beta, Spin& spin_y){
//     flt cdot = 2 * beta * (spin_r | spin_x) * (spin_r | spin_y);
//     flt activate_prob;
//     F64 active = 1.0 - std::exp(min(F64(cdot), 0.0));
//     flt p = rng::rand_f64();
//     return (p <= active);
// }

// bool activate_bond( Spin& spin_x, Spin& spin_r, flt beta, Spin& spin_y){
//     flt cdot = 2*beta*(spin_r | spin_x)*(spin_r | spin_y);
//     //flt activate_prob;
//     F64 active = 1.0 - std::exp(min(F64(cdot), 0.0));
//     flt p = rng::randflt();
//     bool a = false;
//     if (p <= active){bool a = true;}
//     return (a);
// }

// bool activate_bond(Spin& spin_x, Spin& spin_r, flt beta, Spin& spin_y){
//     flt cdot = spin_x | spin_r;
//     Spin spin_x_r = Spin::get_random();
//     spin_x_r = spin_x - (2.0f * cdot)*spin_r;
//     flt Z1 = spin_x_r | spin_y;
//     flt Z2 = spin_x | spin_y;
//     F64 active = min(0.0, 1 - std::exp(beta *( F64(Z1) - F64(Z2)) ));
//     flt p = rng::randflt();
//     return (p <= active);
// }

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
    int x = rng::rand_int_range(0,Lx);
    int y = rng::rand_int_range(0,Ly);
    int z = rng::rand_int_range(0,Lz);
    
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

    }

    //Compute cluster size
    size_t clusterSize = cluster.size();

    if (clusterSize == 0){
        return -1;
    }

    return clusterSize;
}

//bool wolff(Lattice& lattice);
/*
performs the wolff algoritm on the lattice

- L: Lattice
- returns: succeptibility if it sucseeds
- can throw
*/

flt wolff(Lattice &lattice, flt T, flt J, flt MaxTime, u64 MaxSteps){

    flt beta = Beta(T);
    measure::Timer watch;

    Array<int> clusters;
    uint nRuns = 0;

    //Run MaxSteps wolff steps or until the maximal time has been reached
    for (uint i = 0; i <= MaxSteps; ++i){
        uint clusterSize = wolf_algorithm(lattice, beta);

        clusters.push_back(clusterSize);

        ++nRuns;
        // Check if maximum time has been reached
        if (watch.time() >= MaxTime) {
            break; // Stop simulation if maximum time reached
        }
    }

    //Compute total cluster size
    uint totalClusterSize = 0;
    for (int size : clusters) {
        totalClusterSize += size;
    }
    
    //Compute mean cluster size
    flt averageClusterSize = static_cast<flt>(totalClusterSize)/nRuns;
    
    //Compute susceptibility, does this still hold, without a factor?
    flt susceptibility = averageClusterSize;

    return susceptibility;
}

