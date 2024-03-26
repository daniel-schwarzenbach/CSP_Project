#ifndef ADAPTIVE_METROPOLIS_HPP
#define ADAPTIVE_METROPOLIS_HPP

#include <Base.h++>
#include <Lattice3d.h++>
#include <Spin.h++>


/*
performs the metropolis algoritm on the lattice

- L: Lattice
- returns: if it sucseeds
- can throw
*/
bool adaptive_metropolis(Lattice3d<Spin> & lattice);

#endif