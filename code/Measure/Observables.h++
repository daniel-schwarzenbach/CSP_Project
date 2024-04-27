#ifndef __OBSERVABLES_H__
#define __OBSERVABLES_H__

#include <Heisenberg.h++>

namespace measure
{

    /*
    calculate the magnetization of all the spins inside the lattice

    / @brief
    / @param lattice our 3d lattice, where to perform the simulation on
    / @return the non normalized magnetization of the lattice
    */
    Vector3 get_magnetization(const Lattice &lattice);

    /*
    calculate the energy of all the spins inside the lattice

    / @brief
    / @param lattice our 3d lattice, where to perform the simulation on
    / @return the energy of the lattice
    */
    F64 get_energy(const Lattice &lattice, Vector3 h_vec = {0, 0, 0}, F64 J = 1);

}

#endif // __OBSERVABLES_H__