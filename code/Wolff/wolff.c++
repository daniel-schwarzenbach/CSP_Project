#include <Wolff/wolff.h++>
#include <Measure/Timer.h++>

//Function to flip the spin
void flip_spin(Spin& spin_r, Spin& spin_x){
    flt cdot = spin_x | spin_r;
    spin_x = spin_x - (2.0f * cdot)*spin_r;
}

bool activate_bond( Spin& spin_x, Spin& spin_r, flt beta,Spin& spin_y){
    flt cdot = 2 * beta * (spin_r | spin_x) * (spin_r | spin_y);
    flt active = 1.0 - std::exp(min(cdot, 0.0));
    flt p = rng::rand_uniform();
    return (p <= active);
}



void check_neighbor(Lattice3D<Spin>& lattice, Index ind, 
                    Spin& spin_x, Spin& spin_r, 
                    Lattice3D<bool>& visited, 
                    Array<Index>& stack, Array<Index>& cluster, 
                    flt const& beta){
    Spin& spin_y = lattice(ind); //Define spin sigma_y
    //If Bond is activated...
    if (activate_bond(spin_x, spin_r, beta, spin_y)){
        flip_spin(spin_r, spin_y); //...flip spin
        stack.push_back(ind); // ...add to stack 
        cluster.push_back(ind); // ...add to cluster (mark y)
        visited.set(ind,true); //Mark as visited
    }
}

int wolf_algorithm(Lattice3D<Spin>& lattice, flt const& beta){

    int Lx = lattice.Lx();
    int Ly = lattice.Ly();
    int Lz = lattice.Lz();

    //Create vector that checks whether the site has been checked
    // Array<Array<Array<bool>>> visited = checked(Lx, Ly, Lz);
    // Array<bool> visited(false,Lx*Ly*Lz + 1);
    // visited[Lx*Ly*Lz] = true;
    Lattice3D<bool> visited = 
            Lattice3D<bool>::constant_lattice(Lx,Lz,Lz,false);
    visited.set_boundary_conditions(
            lattice.get_boundary_conditions());

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
    Spin& spin_first = lattice(x,y,z);

    //Flip sigma_x and mark x
    flip_spin(spin_r, spin_first);

    //Add spin_x to stack & cluster, mark site as checked
    //stack.push_back({x,y,z});
    stack.push_back({x,y,z});
    cluster.push_back({x,y,z});
    visited.set(x,y,z,true);
    //Iterate over nearest neighbors until stack is empty, i.e. no newly adjoined sites
    while(!stack.empty()){
        
        Index current = stack.back();
        stack.pop_back();

        //Get current lattice position
        int cx = current[0];
        int cy = current[1];
        int cz = current[2];

        Spin& spin_x = lattice(cx,cy,cz);

        //Visit neighboring sites
        Array<Index> neighbors = {
        {cx + 1, cy, cz}, {cx - 1, cy, cz}, 
        {cx, cy + 1, cz}, {cx, cy - 1, cz}, 
        {cx, cy, cz + 1}, {cx, cy, cz - 1}};
        for (Index neighbor : neighbors){
            if(!visited.get(neighbor)){ 
                check_neighbor(lattice, neighbor, spin_x, spin_r, 
                visited, stack, cluster, beta); 
            }
        }

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

flt wolff(Lattice3D<Spin> &lattice, flt const& T, flt const& J, 
          flt const& MaxTime, u64 const& MaxSteps, Spin const& h){

    flt beta = Beta(T);
    measure::Timer watch;

    Array<int> clusters(0);
    u64 nRuns = 0;

    //Run MaxSteps wolff steps or until the maximal time has been reached
    for (; nRuns < MaxSteps; ++nRuns){
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

