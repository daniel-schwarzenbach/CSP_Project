#ifndef LATTICE3D_HPP
#define LATTICE3D_HPP
#include <Base.h++>

/*
mathematical correct modulo

/ @brief
/ @param i int
/ @param u uint < 2³¹
/ @return i mod u
*/
inline uint modulo(int const &i, uint u)
{
    constexpr uint max_u = 1 << 31;
    // u = min(max_u, u); to expensive
    return (i % static_cast<int>(u) + static_cast<int>(u)) % static_cast<int>(u);
}

// Lattice Containder
/*

Acsess
T x = lattice<T>(x,y,z)

*/
template <class T>
class LatticeSerial
{
private:
    // datavector
    vector<T> data;
    // dimentions
    uint _Lx, _Ly, _Lz;
    BC bc = BC::Periodic;

public:
    // // temperature
    // flt T = 0;
    // // interaction
    // flt J = 1;

    // size of the lattice in x-direction
    inline uint Lx() const { return _Lx; }
    // size of the lattice in y-direction
    inline uint Ly() const { return _Ly; }
    // size of the lattice in z-direction
    inline uint Lz() const { return _Lz; }

    // get boundary condition
    inline BC get_boundary_conditions() const { return bc; }

    void set_boundary_conditions(BC bc_) { bc = bc_; }
    // acess operator const
    inline T operator()(int x, int y, int z) const
    {
        switch (bc)
        {
        case BC::Dirichlet:
            if (x >= 0 && x < _Lx &&
                y >= 0 && y < _Ly &&
                z >= 0 && z < _Lz)
            {
                return data.at(x * _Ly * _Lz + y * _Lz + z);
            }
            else
            {
                return T(0);
            }
            break;

        default:
            uint x_ = modulo(x, _Lx);
            uint y_ = modulo(y, _Ly);
            uint z_ = modulo(x, _Lz);
            return data.at(x_ * _Ly * _Lz + y_ * _Lz + z_);
            break;
        }
    }
    // acess operator refrence
    inline T &operator()(int x, int y, int z)
    {
        switch (bc)
        {
        case BC::Dirichlet:
            if (x >= 0 && x < _Lx &&
                y >= 0 && y < _Ly &&
                z >= 0 && z < _Lz)
            {
                return data.at(x * _Ly * _Lz + y * _Lz + z);
            }
            else
            {
                return data.at(_Lx * _Ly * _Lz) = T(0);
            }
            break;

        default:
            uint x_ = modulo(x, _Lx);
            uint y_ = modulo(y, _Ly);
            uint z_ = modulo(z, _Lz);
            return data.at(x_ * _Ly * _Lz + y_ * _Lz + z_);
            ;
            break;
        }
    }

    LatticeSerial(uint Lx_, uint Ly_, uint Lz_)
        : _Lx(Lx_), _Ly(Ly_), _Lz(Lz_), data(Lx_ * Ly_ * Lz_ + 1)
    {
        data.resize(Lx_ * Ly_ * Lz_ + 1);
        data.shrink_to_fit();
        for (uint x = 0; x < _Lx; ++x)
        {
            for (uint y = 0; y < _Ly; ++y)
            {
                for (uint z = 0; z < _Lz; ++z)
                {
                    (*this)(x, y, z) = T();
                }
            }
        }
    }
    LatticeSerial(LatticeSerial &other) = default;
    LatticeSerial(LatticeSerial const &other) = default;

    bool regenerate(uint seed = 42)
    {
        srand(seed);
        for (uint x = 0; x < _Lx; ++x)
        {
            for (uint y = 0; y < _Ly; ++y)
            {
                for (uint z = 0; z < _Lz; ++z)
                {
                    (*this)(x, y, z) = T();
                }
            }
        }
        return true;
    }

    int memory_size() const
    {
        return data.size() * sizeof(T);
    }

    static LatticeSerial constant_lattice(uint Lx_,uint Ly_,uint Lz_, 
                                          T const &value)
    {
        LatticeSerial lattice(Lx_, Ly_, Lz_);
        for (uint x = 0; x < Lx_; ++x)
        {
            for (uint y = 0; y < Ly_; ++y)
            {
                for (uint z = 0; z < Lz_; ++z)
                {
                    lattice(x, y, z) = value;
                }
            }
        }
        return lattice;
    }

    static LatticeSerial random_lattice(uint Lx_,uint Ly_,uint Lz_, 
                                        uint seed = 42)
    {
        LatticeSerial lattice(Lx_, Ly_, Lz_);
        srand(seed);
        for (uint x = 0; x < Lx_; ++x)
        {
            for (uint y = 0; y < Ly_; ++y)
            {
                for (uint z = 0; z < Lz_; ++z)
                {
                    lattice(x, y, z) = T::get_random();
                }
            }
        }
        return lattice;
    }
};

#include <Spin.h++>
template class LatticeSerial<SpinCartesian>;
template class LatticeSerial<SpinPolar>;

#endif