#include "wolf.h++"

using flt = float;

/*
performs the wolf algoritm on the lattice

- L: Lattice
- returns: succeptibility if it sucseeds
- can throw
*/

flt wolf(Lattice & lattice){
    // to implement

    flt T = 30.0f;
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