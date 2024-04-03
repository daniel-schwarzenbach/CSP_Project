#include "Observables.h++"

flt measure::get_magnetization(const Lattice& lattice){
    // TODO
    // this is just an example!!!!
    flt magnetization = 0;
    for (int x = 0; x < lattice.Lx(); x++){
        for (int y = 0; y < lattice.Ly(); y++){
            for (int z = 0; z < lattice.Lz(); z++){
                magnetization += lattice(x,y,z).x();
            }
        }
    }
    return magnetization;
}

flt measure::get_energy(const Lattice& lattice){
    // TODO
    // this is just an example!!!!
    flt energy = 0;
    for (int x = 0; x < lattice.Lx(); x++){
        for (int y = 0; y < lattice.Ly(); y++){
            for (int z = 0; z < lattice.Lz(); z++){
                energy += lattice(x,y,z) | lattice(x+1,y,z);
                energy += lattice(x,y,z) | lattice(x,y+1,z);
                energy += lattice(x,y,z) | lattice(x,y,z+1);
            }
        }
    }
    return -energy;
}
flt measure::get_specific_heat(const Lattice& lattice){
    // TODO
    return 0;
}

vector<flt> measure::get_all_observables(const Lattice& lattice){
    // TODO
    vector<flt> observables(3);
    observables[0] = measure::get_magnetization(lattice);
    observables[1] = measure::get_energy(lattice);
    observables[2] = measure::get_specific_heat(lattice);
    return observables;
}