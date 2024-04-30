#include <Heisenberg.h++>
//using Index = StaticArray<int,3>;

class LatticeBool{
private:
    // datavector
    Array<bool> data;
    // dimentions
    uint _Lx, _Ly, _Lz;
    BC bc = BC::Periodic;
public:
    // size of the lattice in x-direction
    inline uint Lx() const;
    // size of the lattice in y-direction
    inline uint Ly() const;
    // size of the lattice in z-direction
    inline uint Lz() const;

    // get boundary condition
    inline BC get_boundary_conditions() const;

    void set_boundary_conditions(BC bc);
    // acess operator const
    bool get(int x, int y, int z) const;
    // acess operator refrence
    void set(int x, int y, int z, bool to_set);

    void set(Index const& id, bool to_set);

    bool get(Index const& id) const;

    LatticeBool(uint Lx_, uint Ly_, uint Lz_);
    LatticeBool(LatticeBool &other) = default;
    LatticeBool(LatticeBool const &other) = default;


    static LatticeBool constant_lattice(
            uint Lx_, uint Ly_, uint Lz_, bool const &value);

    uint get_total_size() const;
};