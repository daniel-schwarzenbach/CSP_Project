#include <Metropolis/metropolis.h++>
#include <Metropolis/energy_diff.h++>
#include <random>

// Metropolis algorithm //

// This function takes a reference to an initial lattice and performs
// the Metropolis algorithm on this lattice at the specified temperature.
// We can specify the trial move which we want to use. The algorithm
// stops when the max time OR the max number of steps, that are also
// specified by the input, are reached.
using std::min; using std::max;
constexpr flt _eps_ = 1e-200;

inline flt calculate_energy_diff_omp(Lattice& lattice, int  const& x, 
                        int const& y,int const& z, 
                        Spin const& oldSpin, Spin const& newSpin, 
                        flt const& J, Spin const& h, Spin const& k,
                        const Array<Spin>& spinArray){
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
        spin[0] = spinArray[lid][0];
        #pragma omp atomic read
        spin[1] = spinArray[lid][1];
        #pragma omp atomic read
        spin[2] = spinArray[lid][2];
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
constexpr flt changerate = 0.9;

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
    Array<Spin>& spinArray = lattice.get_raw_data();
    uint seed = rng::rand_int_range(0, 4'000'000);
    #pragma omp parallel
    {
    // init random number generator
    std::mt19937 rndmgen(seed + omp_get_thread_num());
    std::normal_distribution<flt> gaussian(0.0, 1.0);
    std::uniform_real_distribution<flt> uniform(0.0, 1.0);
    std::uniform_real_distribution<flt> uniform_signed(0.0, 1.0);
    std::uniform_int_distribution<int> disLx(0, Lx-1);
    std::uniform_int_distribution<int> disLy(0, Ly-1);
    std::uniform_int_distribution<int> disLz(0, Lz-1);

    u64 thread_totalSteps = totalSteps_omp;
    u64 thread_acceptedCount = acceptedCount_omp;
    flt thread_sigma = sigma_omp;
    // cout << INFO; what_is(thread_totalSteps);
    // cout << INFO; what_is(thread_acceptedCount);
    #pragma omp for schedule (dynamic, 100)
    for (u64 step = 0; step < maxSteps; ++step){
        // Choose a random lattice site
        int x = disLx(rndmgen);
        int y = disLy(rndmgen);
        int z = disLz(rndmgen);
        // Get the spin at the chosen site (cartesian)
        Spin spin;
        //#pragma omp atomic update
        {
        uint lid = lattice.get_raw_id(x,y,z);
        // reads in spin is only paratially thread safe
        #pragma omp atomic read
        spin[0] = spinArray[lid][0];
        #pragma omp atomic read
        spin[1] = spinArray[lid][1];
        #pragma omp atomic read
        spin[2] = spinArray[lid][2];
        }

        // Propose spin change based on given trial move
        Spin newSpin;
        // do the requested move
        switch (moveType)
        {
            case MoveType::SpinFlip:{
                newSpin = -spin;
                break;
            }
            case MoveType::Random:{
                f32 nx = uniform_signed(rndmgen);
                f32 ny = uniform_signed(rndmgen);
                f32 nz = uniform_signed(rndmgen);
                Spin randomSpin{nx,ny,nz};
                newSpin = randomSpin.normalized();
                break;
            }
            case MoveType::SmallStep:{
                constexpr flt openingAngle = 0.2;
                flt theta = rng::rand_uniform_singed() * openingAngle;
                flt phi = rng::rand_uniform() * _2pi_;
                Vector3 randomPole;
                randomPole << std::sin(theta) * std::cos(phi),
                        std::sin(theta) * std::sin(phi),
                        std::cos(theta);
                Vector3 northPole(0, 0, 1);
                Eigen::Quaternion<f32> rotationToOriginal =
                        Eigen::Quaternion<f32>::FromTwoVectors(
                        northPole, spin);
                newSpin = rotationToOriginal * randomPole;
                break;
            }
            case MoveType::Addaptive:{
                f32 nx = gaussian(rndmgen);
                f32 ny = gaussian(rndmgen);
                f32 nz = gaussian(rndmgen);
                Spin addSpin{nx,ny,nz};
                newSpin = (spin + thread_sigma*addSpin).normalized();
                ++thread_totalSteps;
                break;
            }
            
        }
        // if (newSpin == spin){
        //     cerr << ERROR << "fatal error" <<endl;
        //     cout <<newSpin << " == " << spin << endl;

        // }
        // Calculate energy difference
        flt deltaE = calculate_energy_diff_omp(lattice, x, y, z, spin,
                                            newSpin, J, h, k, 
                                            spinArray);
        
        // Acceptance condition
        if (deltaE <= 0 || uniform(rndmgen) < exp(-deltaE * beta))
        { // Boltzmann constant k is
            // normalized with interaction strength J in this implementation
            // Accept the new configuration
            {
            uint lid = lattice.get_raw_id(x,y,z);
            // reads in newSpin is only paratially thread safe
            #pragma omp atomic write
            spinArray[lid][0] = newSpin[0];
            #pragma omp atomic write
            spinArray[lid][1] = newSpin[1];
            #pragma omp atomic write
            spinArray[lid][2] = newSpin[2];
            }

            if(moveType == MoveType::Addaptive){
                //thread_sigma /= chanerate;
                ++thread_acceptedCount;
                // Update acceptance rate
                const flt R =   thread_acceptedCount/
                                (thread_totalSteps+1.0);
                // Calculate update factor
                const flt f = 0.5 / (1.0 - R + _eps_);
                // Update sigma   
                thread_sigma=min(maxFactor_omp,thread_sigma*f);
            }
        }
        // else if(moveType == MoveType::Addaptive){
        //     thread_sigma *= chanerate;
        // }

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
         
        {
        if(omp_get_thread_num() == 0){
    
        #pragma omp atomic write
        totalSteps_omp = thread_totalSteps;
        #pragma omp atomic write
        acceptedCount_omp = thread_acceptedCount;
        #pragma omp atomic write
        sigma_omp = thread_sigma;
        }
        } // end critical
    }
    } // end parrallel
    return true;
}