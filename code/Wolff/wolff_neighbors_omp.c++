#include <Wolff/wolff.h++>
#include <Measure/Timer.h++>
#include <thread>
#include <random>
#include <mutex>

// Function to flip the spin
inline void flip_spin(Spin const &randomSpin, Spin &startSpin)
{
    flt cdot = startSpin | randomSpin;
    startSpin = startSpin - (2.0 * cdot) * randomSpin;
}

inline flt activate_bond(flt const &J, Spin const &spinX,
                         Spin const &randomSpin,
                         flt const &beta, Spin const &spinY)
{
    flt energy = J * 2.0 * beta * (randomSpin | spinX) * (randomSpin | spinY);
    return 1.0 - exp(min(energy, 0.0));
}

inline u64 wolf_algorithm_neighbours_omp(Lattice &lattice,
                                         flt const &beta, flt const &J, Array<Spin> &spinArray)
{
    int Lx = lattice.Lx();
    int Ly = lattice.Ly();
    int Lz = lattice.Lz();

    // Create vector that checks whether the site has been checked
    Lattice3D<bool> visited =
        Lattice3D<bool>::constant_lattice(Lx, Lz, Lz, false);
    visited.set_boundary_conditions(
        lattice.get_boundary_conditions());

    Stack<Index> stack;
    u64 clusterSize = 0;

    // set up rngs
    uint seed = rng::rand_int_range(0, 4'000'000);
    uint numThreads = omp_get_max_threads();
    Array<std::mt19937> rngs(numThreads);
    std::uniform_real_distribution<flt> uniform(0.0, 1.0);

    #pragma omp parallel for
    for (uint i = 0; i < numThreads; ++i)
    {
        uint threadNum = omp_get_thread_num();
        rngs[threadNum] = std::mt19937(seed + threadNum);
    }

    Spin randomSpin = Spin::get_random();

    // Choose random lattice site as first point of cluster
    int sx = rng::rand_int_range(0, Lx);
    int sy = rng::rand_int_range(0, Ly);
    int sz = rng::rand_int_range(0, Lz);

    stack.push({sx, sy, sz});
    ++clusterSize;
    visited.set(sx, sy, sz, true);

    // Define spin_first to be flipped, first point of the cluster
    Spin &startSpin = lattice(sx, sy, sz);

    // Flip spin_first and mark Index {x,y,z}
    flip_spin(randomSpin, startSpin);

    while (!stack.empty())
    {
        Index current = stack.top();
        stack.pop();
        // Get current lattice position
        int x = current[0];
        int y = current[1];
        int z = current[2];

        Spin currentSpin = lattice(current);
        Array<Index> neighbors = {
            {x - 1, y, z}, {x, y - 1, z}, {x, y, z - 1}, 
            {x + 1, y, z}, {x, y + 1, z}, {x, y, z + 1}};
        // Array<Index> neighbors = {
        //         {x, y, z-1}, {x, y, z+1},
        //         {x, y-1, z-1}, {x, y-1, z}, {x, y-1, z+1},
        //         {x, y+1, z-1}, {x, y+1, z}, {x, y+1, z+1},

        //         {x+1, y, z-1}, {x+1, y, z+1}, {x+1, y, z},
        //         {x+1, y-1, z-1}, {x+1, y-1, z}, {x+1, y-1, z+1},
        //         {x+1, y+1, z-1}, {x+1, y+1, z}, {x+1, y+1, z+1},

        //         {x-1, y, z-1}, {x-1, y, z+1}, {x-1, y, z},
        //         {x-1, y-1, z-1}, {x-1, y-1, z}, {x-1, y-1, z+1},
        //         {x-1, y+1, z-1}, {x-1, y+1, z}, {x-1, y+1, z+1}
        // };
        uint numNeighbors = neighbors.size();
        Array<bool> shouldBeAddetToStack(numNeighbors, false);

        #pragma omp parallel for
        for (uint i = 0; i < numNeighbors; ++i)
        {
            uint threadNum = omp_get_thread_num();
            Index const& neighbor = neighbors[i];

            if (!visited.get(neighbor))
            {
                Spin& neighborSpin = lattice(neighbor);

                // If Bond is activated...
                if (uniform(rngs[threadNum]) <=
                    activate_bond(J,
                                  currentSpin,
                                  randomSpin, beta, neighborSpin))
                {
                    flip_spin(randomSpin, neighborSpin);
                    shouldBeAddetToStack[i] = true;
                    visited.set(neighbor, true);
                    #pragma omp atomic update
                    ++clusterSize;
                } // end if activated
            } // end if !wasVisited
        } // end for parallel neighbors

        // add neighbors to stack
        for (uint i = 0; i < numNeighbors; ++i)
        {
            if (shouldBeAddetToStack[i])
            {
                stack.push(neighbors[i]);
            }
        }
    } // end while(work to do)
    return clusterSize;
}

/*
performs the wolff algoritm on the lattice

- L: Lattice
- returns: succeptibility if it sucseeds
- can throw
*/

flt wolff_neighbours_omp(Lattice &lattice, flt const &T, flt const &J,
              flt const &MaxTime, u64 const &MaxSteps)
{
    // to implement
    flt beta = Beta(T);
    measure::Timer watch;
    Array<flt> clusters(0);
    Array<Spin> &spinArray = lattice.get_raw_data();
    u64 nRuns = 0;

    for (; nRuns <= MaxSteps; ++nRuns)
    {
        uint size = wolf_algorithm_neighbours_omp(lattice, beta, J,
                                                  spinArray);
        clusters.push_back(size);
        // Check if maximum time has been reached
        if (watch.time() >= MaxTime)
        {
            break; // Stop simulation if maximum time reached
        }
    }
    return mean(clusters);
}