#ifndef LATTICE3D_HPP
#define LATTICE3D_HPP
#include <Base.h++>

using Index = StaticArray<int, 3>;

/*
mathematical correct modulo

/ @brief
/ @param i int
/ @param u uint < 2³¹
/ @return i mod u
*/
uint modulo(int const &i, uint u);

// Lattice Containder
/*


T value = lattice<T>(x,y,z)

*/
template <class T>
class Lattice3d
{
private:
    // datavector
    Array<T> data;
    // dimentions
    uint Lx_, Ly_, Lz_;
    BC bc = BC::Periodic;
    T zero_element;
    T dummy_element;
public:

    // size of the lattice in x-direction
    uint Lx() const;
    // size of the lattice in y-direction
    uint Ly() const;
    // size of the lattice in z-direction
    uint Lz() const;

    // get boundary condition
    BC get_boundary_conditions() const;

    // set value of the BC::_0
    void set_zero_element(T const& zero);

    // set the desired boundary condition
    void set_boundary_conditions(BC bc);
    // acess operator const, doesn't work for bools!
    T operator()(int x, int y, int z) const;
    // acess operator refrence, doesn't work for bools!
    T &operator()(int x, int y, int z);
    // acess operator refrence, doesn't work for bools!
    T &operator()(Index const& id);
    // acess operator refrence, doesn't work for bools!
    T operator()(Index const& id) const;
    // set operator, works for bools
    void set(int const& x, int  const& y, int const& z, T const& v);
    // set operator, works for bools
    void set(Index const& id, T const& v);
    // set operator, works for bools
    T get(int const& x, int  const& y, int const& z) const;
    // set operator, works for bools
    T get(Index const& id) const;
    // consturctor
    Lattice3d(uint Lx, uint Ly, uint Lz);
    // copy constructor
    Lattice3d(Lattice3d &other) = default;
    Lattice3d(Lattice3d const &other) = default;

    // randomizes the Lattice, same effect as Lattice::random_lattice
    bool randomize();

    static Lattice3d constant_lattice(uint Lx, uint Ly, uint Lz,
                                      T const &value);

    // randomizes the Lattice, same effect as Lattice::random_lattice
    static Lattice3d random_lattice(uint Lx_, uint Ly_, uint Lz_);

    // size of the entire lattice
    uint get_total_size() const;
};

#include <Spin.h++>

#endif