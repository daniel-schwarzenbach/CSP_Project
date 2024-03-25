#ifndef LATTICE3D_HPP
#define LATTICE3D_HPP
#include <Base.h++>

enum BC{
    Dirichlet,
    Periodic,
    Neumann
};

template<class T>
class Lattice3d{
private:
    vector<T> data;

public:
    BC bc = BC::Periodic;
    Lattice3d(uint Lx, uint Ly, uint Lz, uint seed=42): data(){}
    Lattice3d(Lattice3d& other) = default;
    Lattice3d(Lattice3d const& other) = default;
};

#endif