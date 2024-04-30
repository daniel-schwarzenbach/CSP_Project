#include <Wolff/LatticeBool.h++>

// size of the lattice in x-direction
uint LatticeBool::Lx() const { return _Lx; }
// size of the lattice in y-direction
uint LatticeBool::Ly() const { return _Ly; }
// size of the lattice in z-direction
uint LatticeBool::Lz() const { return _Lz; }

// get boundary condition
BC LatticeBool::get_boundary_conditions() const { return bc; }

void LatticeBool::set_boundary_conditions(BC bc_) { bc = bc_; }
// acess operator const
bool LatticeBool::get(int x, int y, int z) const
{
    switch (bc)
    {
    case BC::_0:
        if (x >= 0 && x < _Lx &&
            y >= 0 && y < _Ly &&
            z >= 0 && z < _Lz)
        {
            return data.at(x * _Ly * _Lz + y * _Lz + z);
        }
        else
        {
            return true;
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
void LatticeBool::set(int x, int y, int z, bool to_set)
{
    switch (bc)
    {
    case BC::_0:
        if (x >= 0 && x < _Lx &&
            y >= 0 && y < _Ly &&
            z >= 0 && z < _Lz)
        {
            data.at(x * _Ly * _Lz + y * _Lz + z) = to_set;
        }
        else
        {
            data.at(_Lx * _Ly * _Lz) = to_set;
        }
        break;

    default:
        uint x_ = modulo(x, _Lx);
        uint y_ = modulo(y, _Ly);
        uint z_ = modulo(z, _Lz);
        data.at(x_ * _Ly * _Lz + y_ * _Lz + z_) = to_set;
        break;
    }
}

void LatticeBool::set(Index const& id, bool to_set){
    set(id[0], id[1], id[2], to_set);
}

bool LatticeBool::get(Index const& id) const{
    return get(id[0], id[1], id[2]);
}

LatticeBool::LatticeBool(uint Lx_, uint Ly_, uint Lz_)
    : _Lx(Lx_), _Ly(Ly_), _Lz(Lz_), data(Lx_ * Ly_ * Lz_ + 1)
{
    data.resize(Lx_ * Ly_ * Lz_ + 1);
    data.shrink_to_fit();
}


LatticeBool LatticeBool::constant_lattice(uint Lx_, uint Ly_, uint Lz_,
                                    bool const &value)
{
    LatticeBool lattice(Lx_, Ly_, Lz_);
    #pragma omp parallel for collapse(3)
    for (uint x = 0; x < Lx_; ++x)
    {
        for (uint y = 0; y < Ly_; ++y)
        {
            for (uint z = 0; z < Lz_; ++z)
            {
                lattice.set(x, y, z, value);
            }
        }
    }
    return lattice;
}

uint LatticeBool::get_total_size() const
{
    return _Lx * _Ly * _Lz;
}