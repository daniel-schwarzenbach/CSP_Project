#include <Wolff/wolff.h++>
#include <Measure/Timer.h++>
#include <thread>
#include <random>

constexpr flt max_sigma = 1;
flt sigma_wolff = 1;


//Function to flip the spin
inline void flip_spin(Spin const& randomSpin, Spin& startSpin){
    flt cdot = startSpin | randomSpin;
    startSpin = startSpin - (2.0 * cdot)*randomSpin;
}

inline flt activate_bond(  flt const& J, Spin const& spinX,
                            Spin const& randomSpin, 
                            flt const& beta, Spin const& spinY)
{
    flt energy = J * 2.0 * beta 
            * (randomSpin|spinX) * (randomSpin|spinY);
    return 1.0 - exp(min(energy, 0.0));
}


inline u64 wolf_algorithm_shotgun_omp(Lattice &lattice, 
                    flt const& beta, 
                    flt const &J,
                    Array<Spin>& spinArray)
{
    int Lx = lattice.Lx();
    int Ly = lattice.Ly();
    int Lz = lattice.Lz();

    // Create vector that checks whether the site has been checked
    Lattice3D<u8> visited = 
            Lattice3D<u8>::constant_lattice(Lx,Lz,Lz,false);
    visited.set_boundary_conditions(
            lattice.get_boundary_conditions());
    visited.set_zero_element(true);
    Array<u8> u8Array = visited.get_raw_data();
    uint clusterSize = 0;

    // Choose random reflection
    Spin randomSpin = Spin::get_random();

    // Iterate over nearest neighbors until stack is empty, i.e. 
    // no newly adjoined sites
    uint seed = rng::rand_int_range(0, 4'000'000);

    #pragma omp parallel
    { // parallel
    // init random number generator for every thread
    std::mt19937 rndmgen(seed + omp_get_thread_num());

    std::uniform_real_distribution<flt> uniform(0.0, 1.0);
    std::uniform_int_distribution<int> disLx(0, Lx-1);
    std::uniform_int_distribution<int> disLy(0, Ly-1);
    std::uniform_int_distribution<int> disLz(0, Lz-1);

        // Choose random lattice site as first point of cluster
    int sx = disLx(rndmgen);
    int sy = disLy(rndmgen);
    int sz = disLz(rndmgen);

    // Define startSpin to be flipped, first point of the cluster
    Spin startSpin;
    uint sid = lattice.get_raw_id(sx,sy,sz);
    
    #pragma omp atomic read
    startSpin[0] = spinArray[sid][0];
    #pragma omp atomic read
    startSpin[1] = spinArray[sid][1];
    #pragma omp atomic read
    startSpin[2] = spinArray[sid][2];
    
    // Flip sigma_x and mark x
    flip_spin(randomSpin, startSpin);

    #pragma omp atomic write
    spinArray[sid][0] = startSpin[0] ;
    #pragma omp atomic write
    spinArray[sid][1] = startSpin[1];
    #pragma omp atomic write
    spinArray[sid][2] = startSpin[2];

    Array<Index> local_stack(0);
    // Add startSpin to stack & cluster, mark site as checked
    local_stack.push_back({sx, sy, sz});
    #pragma omp atomic write
    u8Array[sid] = true;

    while (!local_stack.empty())
    {
        Index current = local_stack.back();
        local_stack.pop_back();
        
        // Get current lattice position
        int x = current[0];
        int y = current[1];
        int z = current[2];

        uint lid = lattice.get_raw_id(current);
        Spin currentSpin;
        #pragma omp atomic read
        currentSpin[0] = spinArray[lid][0];
        #pragma omp atomic read
        currentSpin[1] = spinArray[lid][1];
        #pragma omp atomic read
        currentSpin[2] = spinArray[lid][2];

        Array<Index> neighbors = {
            {x - 1, y, z}, {x, y - 1, z}, {x, y, z - 1}, 
            {x + 1, y, z}, {x, y + 1, z}, {x, y, z + 1}};
        for (Index neighbor : neighbors)
        {
            u8 wasVisited;
            uint nid = lattice.get_raw_id(neighbor);
            #pragma omp atomic read
            wasVisited = u8Array[nid];
            
            
            if (!wasVisited)
            {
                Spin neighborSpin;
                #pragma omp atomic read
                neighborSpin[0] = spinArray[nid][0];
                #pragma omp atomic read
                neighborSpin[1] = spinArray[nid][1];
                #pragma omp atomic read
                neighborSpin[2] = spinArray[nid][2];

                // If Bond is activated...
                if (uniform(rndmgen) <= activate_bond(J, currentSpin, 
                        randomSpin, beta, neighborSpin))
                {

                    flip_spin(randomSpin, neighborSpin);
                    #pragma omp atomic write
                    spinArray[nid][0] = neighborSpin[0] ;
                    #pragma omp atomic write
                    spinArray[nid][1] = neighborSpin[1];
                    #pragma omp atomic write
                    spinArray[nid][2] = neighborSpin[2];

                    local_stack.push_back(neighbor);
                    #pragma omp atomic write
                    u8Array[nid] = true;

                    #pragma omp atomic update
                    ++clusterSize;
                }
            }
        }
    }
    } // end parallel
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
    Array<flt> clusters(0);
    Array<Spin>& spinArray = lattice.get_raw_data();
    u64 nRuns = 0;

    for (; nRuns <= MaxSteps; ++nRuns)
    {
        uint clusterSize = wolf_algorithm_shotgun_omp(lattice, 
                beta, J,spinArray);
        clusters.push_back(clusterSize);
        // Check if maximum time has been reached
        if (watch.time() >= MaxTime)
        {
            break; // Stop simulation if maximum time reached
        }
    }
    return mean(clusters);
}