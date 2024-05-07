#include <Wolff/wolff.h++>
#include <Measure/Timer.h++>
#include <thread>



//Function to flip the spin
inline void flip_spin(Spin& spin_r, Spin& spin_x){
    flt cdot = spin_x | spin_r;
    spin_x = spin_x - (2.0f * cdot)*spin_r;
}

bool activate_bond(flt const& J, Spin const& spin_x, Spin const& spin_r, 
                    flt const& beta, Spin const& spin_y){
    flt cdot = J *2 * beta * (spin_r | spin_x) * (spin_r | spin_y);
    flt active = 1.0 - std::exp(min(cdot, 0.0));
    flt p = rng::rand_uniform();
    return (p <= active);
}


int wolf_algorithm(Lattice &lattice, flt const& beta, flt const &J,
                    Array<Spin>& spinArray)
{
    int Lx = lattice.Lx();
    int Ly = lattice.Ly();
    int Lz = lattice.Lz();

    // Create vector that checks whether the site has been checked
    Array<bool> visited(false,Lx*Ly*Lz + 1);
    visited[Lx*Ly*Lz] = true;

    // Define stack for adding and removing lattice sites that are flipped, continue until it is empty (no new sites were added)
    Array<Index> stack(0);

    // Define cluster for adding the lattice sites that belong to the cluster (for computing average lattice size)
    Array<Index> cluster(0);

    // Choose random reflection
    Spin spin_r = Spin::get_random();

    // Choose random lattice site as first point of cluster
    int sx = rng::rand_int_range(0, Lx);
    int sy = rng::rand_int_range(0, Ly);
    int sz = rng::rand_int_range(0, Lz);

    // Define spin_x to be flipped, first point of the cluster
    Spin& spin_x = lattice(sx, sy, sz);

    // Flip sigma_x and mark x
    flip_spin(spin_r, spin_x);

    // Add spin_x to stack & cluster, mark site as checked
    stack.push_back({sx, sy, sz});
    cluster.push_back({sx, sy, sz});
    {
    uint lid = lattice.get_raw_id(sx,sy,sz);
    visited[lid] = true;
    }

    // Iterate over nearest neighbors until stack is empty, i.e. no newly adjoined sites
    int local_count = 3;
    Array<Index> stack_local = stack;
#pragma omp parallel
    { // parallel
        while (!stack_local.empty() || (local_count>0))
        {
            Index current;
            bool isEmpty;
            // only one core at the time
            #pragma omp critical
            {
            isEmpty = stack.empty();
            if (!isEmpty)
            {
                current = stack.back();
                stack.pop_back();
            }
            } // end critical
            if (isEmpty)
            {
                // wait
                std::this_thread::sleep_for(
                    std::chrono::microseconds(1));
                --local_count;
                // updata the steck and
                stack_local = stack;
                continue; // try again
            }
            // Get current lattice position
            int x = current[0];
            int y = current[1];
            int z = current[2];

            uint lid = lattice.get_raw_id(current);
            Spin spin_y;
            {
            #pragma omp atomic read
            spin_y[0] = spinArray[lid][0];
            #pragma omp atomic read
            spin_y[1] = spinArray[lid][1];
            #pragma omp atomic read
            spin_y[2] = spinArray[lid][2];
            }
            Array<Index> neighbors = {
                {x - 1, y, z}, {x, y - 1, z}, {x, y, z - 1}, 
                {x + 1, y, z}, {x, y + 1, z}, {x, y, z + 1}};
            for (Index neighbor : neighbors)
            {
                bool wasVisited;
                uint nid = lattice.get_raw_id(neighbor);
                {
                wasVisited = visited[nid];
                }
                if (!wasVisited)
                {
                    Spin spin_neighbor;
                    {
                    #pragma omp atomic read
                    spin_neighbor[0] = spinArray[nid][0];
                    #pragma omp atomic read
                    spin_neighbor[1] = spinArray[nid][1];
                    #pragma omp atomic read
                    spin_neighbor[2] = spinArray[nid][2];
                    }

                    // If Bond is activated...
                    if (activate_bond(J, spin_x, spin_r, beta,spin_y))
                    {

                        flip_spin(spin_r, spin_neighbor);
                        {
                        #pragma omp atomic write
                        spinArray[nid][0] = spin_neighbor[0] ;
                        #pragma omp atomic write
                        spinArray[nid][1] = spin_neighbor[1];
                        #pragma omp atomic write
                        spinArray[nid][2] = spin_neighbor[2];
                        }

                        #pragma omp critical
                        stack.push_back(neighbor);            // ...add to stack
                        #pragma omp critical
                        cluster.push_back(neighbor);          // ...add to cluster (mark y)
                        {
                        visited[nid] = true;
                        };
                        stack_local = stack;
                    }
                }
            }
        }

    } // end parallel

    // Compute cluster size
    size_t clusterSize = cluster.size();

    if (clusterSize == 0)
    {
        return -1;
    }

    return clusterSize;
}

/*
performs the wolff algoritm on the lattice

- L: Lattice
- returns: succeptibility if it sucseeds
- can throw
*/

flt wolff_omp(Lattice &lattice, flt const &T, flt const &J,
              flt const &MaxTime, u64 const &MaxSteps)
{
    // to implement

    flt beta = Beta(T);
    measure::Timer watch;
    Array<Spin>& spinArray = lattice.get_raw_data();
    Array<int> clusters;
    u64 nRuns = 0;

    for (; nRuns <= MaxSteps; ++nRuns)
    {
        uint clusterSize = wolf_algorithm(lattice, beta, J, spinArray);
        if (clusterSize == -1)
        {
            return false;
            std::cout << "ERROR" << std::endl;
        }
        clusters.push_back(clusterSize);

        // Check if maximum time has been reached
        if (watch.time() >= MaxTime)
        {
            break; // Stop simulation if maximum time reached
        }
    }

    // Compute total cluster size
    uint totalClusterSize = 0;
    for (int size : clusters)
    {
        totalClusterSize += size;
    }

    flt averageClusterSize = static_cast<flt>(totalClusterSize) / nRuns;

    flt susceptibility = averageClusterSize;

    return susceptibility;
}