#ifndef LATTICE3D_HPP
#define LATTICE3D_HPP
#include <Base.h++>
#include <Spin.h++>
#include <type_traits>

using Index = StaticArray<int, 3>;

/*
mathematical correct modulo

/ @brief
/ @param i int
/ @param u uint < 2³¹
/ @return i mod u
*/
inline uint modulo(int const &i, uint u);

// Lattice Container
/*


T value = lattice<T>(x,y,z)

*/
template <class T>
class Lattice3D
{
private:
    // datavector
    Array<T> data;
    // dimentions of the lattic
    uint Lx_, Ly_, Lz_; u64 fullSize;
    // boundary condition
    BC bc = BC::Periodic;

public:
    // return raw data
    Array<T> &get_raw_data();
    // return raw data id
    uint get_raw_id(int const &x, int const &y, int const &z) const;
    // return raw data id
    uint get_raw_id(Index const &index) const;
    // size of the lattice in x-direction
    uint Lx() const;
    // size of the lattice in y-direction
    uint Ly() const;
    // size of the lattice in z-direction
    uint Lz() const;
    // get boundary condition
    BC get_boundary_conditions() const;
    // set value of the BC::_0
    void set_zero_element(T const &zero);
    // set the desired boundary condition
    void set_boundary_conditions(BC bc);
    // acess operator const, doesn't work for bools!
    T operator()(int x, int y, int z) const;
    // acess operator refrence, doesn't work for bools!
    T &operator()(int x, int y, int z);
    // acess operator refrence, doesn't work for bools!
    T &operator()(Index const &id);
    // acess operator refrence, doesn't work for bools!
    T operator()(Index const &id) const;
    // consturctor
    Lattice3D(uint Lx, uint Ly, uint Lz);
    // copy constructor
    Lattice3D(Lattice3D &other) = default;
    Lattice3D(Lattice3D const &other) = default;

    // randomizes the Lattice, same effect as Lattice::random_lattice
    bool randomize();
    bool set_constant(T const &value);

    static Lattice3D constant_lattice(uint Lx, uint Ly, uint Lz,
                                      T const &value);

    // randomizes the Lattice, same effect as Lattice::random_lattice
    static Lattice3D random_lattice(uint Lx, uint Ly, uint Lz);

    // size of the entire lattice
    uint get_total_size() const;
};

template <>
class Lattice3D<bool>
{

private:
    // datavector
    Array<bool> data;
    // dimentions of the lattic
    uint Lx_, Ly_, Lz_; u64 fullSize;
    // boundary condition
    BC bc = BC::Periodic;

public:
    // size of the lattice in x-direction
    uint Lx() const;
    // size of the lattice in y-direction
    uint Ly() const;
    // size of the lattice in z-direction
    uint Lz() const;
    // return raw data id
    uint get_raw_id(int const &x, int const &y, int const &z) const;
    // get boundary condition
    BC get_boundary_conditions() const;
    // set value of the BC::_0
    void set_zero_element(bool const &zero);
    // set the desired boundary condition
    void set_boundary_conditions(BC bc);
    // set operator, works for bools
    void set(int const &x, int const &y, int const &z, bool const &v);
    // set operator, works for bools
    void set(Index const &id, bool const &v);
    // set operator, works for bools
    bool get(int const &x, int const &y, int const &z) const;
    // set operator, works for bools
    bool get(Index const &id) const;
    // consturctor
    Lattice3D(uint Lx, uint Ly, uint Lz);
    // copy constructor
    Lattice3D(Lattice3D &other) = default;
    Lattice3D(Lattice3D const &other) = default;

    // randomizes the Lattice, same effect as Lattice::random_lattice
    bool randomize();
    bool set_constant(bool const &value);

    static Lattice3D constant_lattice(uint Lx, uint Ly, uint Lz,
                                      bool const &value);

    // randomizes the Lattice, same effect as Lattice::random_lattice
    static Lattice3D random_lattice(uint Lx, uint Ly, uint Lz);

    // size of the entire lattice
    uint get_total_size() const;
};

#endif