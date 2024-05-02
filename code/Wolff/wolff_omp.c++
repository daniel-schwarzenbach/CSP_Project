#include <Wolff/wolff.h++>
#include <Measure/Timer.h++>
#include <Wolff/duplicate_functions.h++>
#include <thread>

// Function to activate bond depending on given probability
bool activate_bond(flt J, Spin &spin_x, Spin &spin_r, flt beta, 
                  Spin &spin_y)
{

    flt cdot = 2 * J * beta * (spin_r | spin_x) * (spin_r | spin_y);
    flt activate_prob;
    flt active = 1.0 - std::exp(min(flt(cdot), 0.0));
    flt p = rng::rand_uniform();
    return (p <= active);
}

int wolf_algorithm(Lattice &lattice, f32 beta, flt const &J)
{
    int Lx = lattice.Lx();
    int Ly = lattice.Ly();
    int Lz = lattice.Lz();

    // Create vector that checks whether the site has been checked
    Lattice3d<bool> visited =
        Lattice3d<bool>::constant_lattice(Lx, Ly, Lz, false);
    visited.set_boundary_conditions(
        lattice.get_boundary_conditions());

    // Define stack for adding and removing lattice sites that are flipped, continue until it is empty (no new sites were added)
    Array<Index> stack(0);

    // Define cluster for adding the lattice sites that belong to the cluster (for computing average lattice size)
    Array<Index> cluster(0);

    // Choose random reflection
    Spin spin_r = Spin::get_random();

    // Choose random lattice site as first point of cluster
    int sx = rng::rand_uniform() * Lx;
    int sy = rng::rand_uniform() * Ly;
    int sz = rng::rand_uniform() * Lz;

    // Define spin_x to be flipped, first point of the cluster
    Spin &spin_x = lattice(sx, sy, sz);

    // Flip sigma_x and mark x
    flip_spin(spin_r, spin_x);

    // Add spin_x to stack & cluster, mark site as checked
    // stack.push_back({x,y,z});
    stack.push_back({sx, sy, sz});
    cluster.push_back({sx, sy, sz});
    visited.set(sx, sy, sz, true);

    // Iterate over nearest neighbors until stack is empty, i.e. no newly adjoined sites
    Array<Index> stack_local = stack;
#pragma omp parallel
    { // parallel
        while (!stack_local.empty())
        {
            Index current;
            bool isEmpty;

#pragma omp critical
            {
                isEmpty = stack.empty();
                if (!isEmpty)
                {
                    current = stack.back();
                    stack.pop_back();
                }
            }
            if (isEmpty)
            {
                // wait
                std::this_thread::sleep_for(
                    std::chrono::microseconds(10));
// update local stack
#pragma omp critical
                stack_local = stack;
                continue; // try again
            }
            // Get current lattice position
            int x = current[0];
            int y = current[1];
            int z = current[2];

// Mark as visited
#pragma omp critical
            visited.set(current, true);

            Array<Index> neighbors = {
                {x - 1, y, z}, {x, y - 1, z}, {x, y, z - 1}, 
                {x + 1, y, z}, {x, y + 1, z}, {x, y, z + 1}};
            for (Index neighbor : neighbors)
            {
                bool wasVisited;
#pragma omp critical
                wasVisited = visited.get(neighbor);
                if (!wasVisited)
                {
                    Spin spin_y;
#pragma omp critical
                    spin_y = lattice(neighbor); // Define spin sigma_y

                    // If Bond is activated...
                    if (activate_bond(J, spin_x, spin_r, beta, spin_y))
                    {
#pragma omp critical
                        {
                            flip_spin(spin_r, lattice(neighbor));       //...flip spin
                            stack.push_back(neighbor);   // ...add to stack
                            cluster.push_back(neighbor); // ...add to cluster (mark y)
                            visited.set(neighbor, true); // Mark as visited
                            stack_local = stack;
                        }
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

flt wolff_omp(Lattice &lattice, flt const& T, flt const& J, 
              flt const& MaxTime, uint const& MaxSteps)
{
    // to implement

    flt beta = Beta(T);
    measure::Timer watch;

    Array<int> clusters;
    uint nRuns = 0;

    for (uint i = 0; i <= MaxSteps; ++i)
    {
        uint clusterSize = wolf_algorithm(lattice, beta, J);
        if (clusterSize == -1)
        {
            return false;
            std::cout << "ERROR" << std::endl;
        }
        clusters.push_back(clusterSize);

        ++nRuns;
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