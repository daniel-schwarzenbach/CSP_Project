#ifndef __OBSERVABLES_H__
#define __OBSERVABLES_H__

#include <Heisenberg.h++>

namespace measure{

/*
calculate the magnetization of all the spins inside the lattice

/ @brief
/ @param lattice our 3d lattice, where to perform the simulation on
/ @return the magnetization of the lattice
*/
flt get_magnetization(const Lattice& lattice);

/*
calculate the energy of all the spins inside the lattice

/ @brief
/ @param lattice our 3d lattice, where to perform the simulation on
/ @return the energy of the lattice
*/
flt get_energy(const Lattice& lattice);

/*
calculate the specific heat of all the spins inside the lattice

/ @brief
/ @param lattice our 3d lattice, where to perform the simulation on
/ @return the specific heat of the lattice
*/
flt get_specific_heat(const Lattice& lattice);


/*
calculate all the observables of the lattice

/ @brief
/ @param lattice our 3d lattice, where to perform the simulation on
/ @return a vector with all the observables
*/
vector<flt> get_all_observables(const Lattice& lattice);


} // namespace measure

#endif // __OBSERVABLES_H__