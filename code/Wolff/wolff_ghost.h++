
#ifndef WOLF_GHOST_HPP
#define WOLF_GHOST_HPP

#include <Heisenberg.h++>
#include <algorithm>
#include <cmath>

/*
wolff algorithm for the Heisenberg3D model

/ @brief
/ @param lattice our 3d lattice, where to perform the simulation on
/ @return if the procedure succeds
/ @exception may fail
*/

#include <array>
#include <functional>

//Needed for using std::unordered_set
struct IndexHash {
    std::size_t operator()(const std::array<int, 3>& index) const {
        std::size_t seed = 0;
        std::hash<int> hasher;
        seed ^= hasher(index[0]) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= hasher(index[1]) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= hasher(index[2]) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    }
};


F64 wolff_ghost(Lattice& lattice, F64 T, F64 J, F64 MaxTime, uint MaxSteps, Spin h);

#endif
