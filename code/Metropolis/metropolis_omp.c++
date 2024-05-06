#include <Metropolis/metropolis.h++>
#include <Metropolis/energy_diff.h++>

// Metropolis algorithm //

// This function takes a reference to an initial lattice and performs
// the Metropolis algorithm on this lattice at the specified temperature.
// We can specify the trial move which we want to use. The algorithm
// stops when the max time OR the max number of steps, that are also
// specified by the input, are reached.

flt calculate_energy_diff_omp(Lattice& lattice, int  const& x, 
                        int const& y,int const& z, 
                        Spin const& oldSpin, Spin const& newSpin, 
                        flt const& J, Spin const& h, Spin const& k){
    Array<Spin>& latticeArray = lattice.get_raw_data();
    // Get dimensions of the lattice
    int Lx = lattice.Lx();
    int Ly = lattice.Ly();
    int Lz = lattice.Lz();

    Array<Index> neighbors = {
        {x + 1, y, z}, {x - 1, y, z}, 
        {x, y + 1, z}, {x, y - 1, z}, 
        {x, y, z + 1}, {x, y, z - 1}};
    // Energies of old and new configuration
    flt energyOld = 0.0;
    flt energyNew = 0.0;
    for (Index neighbor : neighbors)
    {
        // Get indices of neighbors
        // Get neighboring spin
        Spin spin;
        {
        uint lid = lattice.get_raw_id(neighbor);
        #pragma omp atomic read
        spin[0] = latticeArray[lid][0];
        #pragma omp atomic read
        spin[1] = latticeArray[lid][1];
        #pragma omp atomic read
        spin[2] = latticeArray[lid][2];
        }

        // Calcualte and add energies
        energyOld += -J * (oldSpin | spin) 
                     - (oldSpin | h) 
                     - pow((oldSpin | k), 2);
        energyNew += -J * (newSpin | spin) 
                     - (newSpin | h) 
                     - pow((newSpin | k), 2);
    }   
    // Calculate energy difference (deltaE)
    flt deltaE = energyNew - energyOld;
    return deltaE;
}

flt sigma_omp = 100;
u64 totalSteps_omp = 0;
u64 acceptedCount_omp = 0;
constexpr flt maxFactor_omp = 60;

void restet_adaptive_omp(){
    flt sigma_omp = 100;
    u64 totalSteps_omp = 0;
    u64 acceptedCount_omp = 0;
}

bool metropolis_omp(Lattice &lattice,
                    flt const& T /*temperature*/,
                    flt const& J /*interaction Strength*/,
                    flt const& maxTimeSeconds,
                    u64 const& maxSteps,
                    Spin const& h,
                    Spin const& k,
                    MoveType const& moveType)
{
    measure::Timer watch;
    const uint Lx = lattice.Lx();
    const uint Ly = lattice.Ly();
    const uint Lz = lattice.Lz();
    const flt beta = Beta(T);
    flt simga_total = 0.0;
    Array<Spin>& latticArray = lattice.get_raw_data();
    #pragma omp parallel
    {
    u64 thread_totalSteps = totalSteps_omp;
    u64 thread_acceptedCount = acceptedCount_omp;
    flt thread_sigma = sigma_omp;
    #pragma omp for schedule (dynamic, 50)
    for (u64 step = 0; step < maxSteps; ++step){
        ++thread_totalSteps;
        // Choose a random lattice site
        int x = rng::rand_int_range(0, Lx);
        int y = rng::rand_int_range(0, Ly);
        int z = rng::rand_int_range(0, Lz);
        // Get the spin at the chosen site (cartesian)
        Spin spin;
        //#pragma omp atomic update
        {
        uint lid = lattice.get_raw_id(x,y,z);
        #pragma omp atomic read
        spin[0] = latticArray[lid][0];
        #pragma omp atomic read
        spin[1] = latticArray[lid][1];
        #pragma omp atomic read
        spin[2] = latticArray[lid][2];
        }

        // Propose spin change based on given trial move
        Spin newSpin = spin;
        // do the requested move
        switch (moveType)
        {
            case MoveType::SpinFlip:
                newSpin.spin_flip_step(); // Spin flip move (reflect the spin)
                break;
            case MoveType::Random:
                newSpin.random_step(); // Random move (generate a random spin)
                break;
            case MoveType::SmallStep:
                newSpin.small_step(0.1); // Small step move (small change around the current spin)
                break;
            case MoveType::Addaptive:
                newSpin.adaptive_step(sigma_omp);
                break;
            }
        // Calculate energy difference
        flt deltaE = calculate_energy_diff_omp(lattice, x, y, z, spin,
                                            newSpin, J, h, k);
        
        // Acceptance condition
        if (deltaE <= 0 || rng::rand_uniform() < exp(-deltaE * beta))
        { // Boltzmann constant k is
            // normalized with interaction strength J in this implementation
            // Accept the new configuration
            {
            uint lid = lattice.get_raw_id(x,y,z);
            #pragma omp atomic write
            latticArray[lid][0] = newSpin[0];
            #pragma omp atomic write
            latticArray[lid][1] = newSpin[1];
            #pragma omp atomic write
            latticArray[lid][2] = newSpin[2];
            }

            if(moveType == MoveType::Addaptive){
                ++thread_acceptedCount;
                // Update acceptance rate
                const flt R = acceptedCount_omp/(totalSteps_omp+1.0);
                // Calculate update factor
                const flt f = 0.5 / (1.0 - R + 1e-18);
                // Update sigma   
                thread_sigma = std::min(maxFactor_omp,thread_sigma*f);
            }
        }

        // Check if maximum time has been reached
        if (watch.time() >= maxTimeSeconds)
        {
            #ifndef WITH_OPENMP
            break;
            #endif
            #pragma omp cancel for // Stop simulation if maximum time reached
        }
    } // end for
    if(moveType == MoveType::Addaptive){
        
        #pragma omp critical  
        {
        totalSteps_omp += thread_totalSteps;
        acceptedCount_omp += thread_acceptedCount;
        simga_total += thread_sigma;
        } // end critical
    }
    } // end parrallel
    sigma_omp = simga_total / omp_get_num_threads();
    return true;
}