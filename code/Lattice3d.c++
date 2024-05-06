#include <Lattice3d.h++>
/*
mathematical correct modulo

/ @brief
/ @param i int
/ @param u uint < 2³¹
/ @return i mod u
*/
inline uint modulo(int const &i, uint u)
{
    return (i % static_cast<int>(u) + static_cast<int>(u)) % static_cast<int>(u);
}
// row access
#define access(ix, iy, iz) ix *Ly_ *Lz_ + iy *Lz_ + iz

// return raw data
template <typename T>
Array<T>& Lattice3d<T>::get_raw_data(){
    return data;
}
// return raw data id
template <typename T>
uint Lattice3d<T>::get_raw_id(
        int const& x, int const& y, int const& z) const
{
        switch (bc)
    {
    case BC::_0:
        if (x >= 0 && x < Lx_ &&
            y >= 0 && y < Ly_ &&
            z >= 0 && z < Lz_)
        {
            return access(x, y, z);
        }
        else
        {
            return Lz_*Ly_*Lx_;
        }
        break;

    default:
        uint x_ = modulo(x, Lx_);
        uint y_ = modulo(y, Ly_);
        uint z_ = modulo(x, Lz_);
        return access(x_, y_, z_);
        break;
    }
}

// return raw data id
template <typename T>
uint Lattice3d<T>::get_raw_id(Index const& index) const{
    return get_raw_id(index[0], index[1], index[2]);
}

// size of the lattice in x-direction
template <typename T>
uint Lattice3d<T>::Lx() const { return Lx_; }
// size of the lattice in y-direction
template <typename T>
uint Lattice3d<T>::Ly() const { return Ly_; }

template <typename T>
uint Lattice3d<T>::Lz() const { return Lz_; }

// get boundary condition
template <typename T>
BC Lattice3d<T>::get_boundary_conditions() const { return bc; }

// set value of the BC::_0
template <typename T>
void Lattice3d<T>::set_zero_element(T const &zero)
{
    zero_element = zero;
}

template <typename T>
void Lattice3d<T>::set_boundary_conditions(BC bc_) { bc = bc_; }
// acess operator const

template <typename T>
T Lattice3d<T>::operator()(int x, int y, int z) const
{
    switch (bc)
    {
    case BC::_0:
        if (x >= 0 && x < Lx_ &&
            y >= 0 && y < Ly_ &&
            z >= 0 && z < Lz_)
        {
            return data.at(access(x, y, z));
        }
        else
        {
            return zero_element;
        }
        break;

    default:
        uint x_ = modulo(x, Lx_);
        uint y_ = modulo(y, Ly_);
        uint z_ = modulo(x, Lz_);
        return data.at(access(x_, y_, z_));
        break;
    }
    return zero_element;
}



template <typename T>
T &Lattice3d<T>::operator()(Index const &id)
{
    return this->operator()(id[0], id[1], id[2]);
}

template <typename T>
T Lattice3d<T>::operator()(Index const &id) const
{
    return this->operator()(id[0], id[1], id[2]);
}



template <typename T>
T &Lattice3d<T>::operator()(int x, int y, int z)
{
    switch (bc)
    {
    case BC::_0:
        if (x >= 0 && x < Lx_ &&
            y >= 0 && y < Ly_ &&
            z >= 0 && z < Lz_)
        {
            return data.at(access(x, y, z));
        }
        else
        {
            data.at(Lx_*Lz_*Ly_) = zero_element;
            return data.at(Lx_*Lz_*Ly_);
        }
    default:
        uint x_ = modulo(x, Lx_);
        uint y_ = modulo(y, Ly_);
        uint z_ = modulo(z, Lz_);
        return data.at(access(x_, y_, z_));
    }
}


template <typename T>
void Lattice3d<T>::set(int const &x, int const &y, int const &z,
                       T const &v)
{
    switch (bc)
    {
    case BC::_0:
        if (x >= 0 && x < Lx_ &&
            y >= 0 && y < Ly_ &&
            z >= 0 && z < Lz_)
        {
            data.at(access(x, y, z)) = v;
        }
        break;
    // BC = periodic
    default:
        uint x_ = modulo(x, Lx_);
        uint y_ = modulo(y, Ly_);
        uint z_ = modulo(z, Lz_);
        data.at(access(x_, y_, z_)) = v;
        break;
    }
}

template <typename T>
void Lattice3d<T>::set(Index const &id, T const &v)
{
    set(id[0], id[1], id[2], v);
}

template <typename T>
T Lattice3d<T>::get(int const &x, int const &y, int const &z) const
{
    return this->operator()(x, y, z);
}

template <typename T>
T Lattice3d<T>::get(Index const &id) const
{
    return this->operator()(id);
}

template <typename T>
Lattice3d<T>::Lattice3d(uint Lx, uint Ly, uint Lz)
    : zero_element(0), Lx_(Lx), Ly_(Ly), Lz_(Lz),
      data(Lx * Ly * Lz +1)
{
    data.resize(Lx * Ly * Lz + 1);
    data[Lx*Ly*Lz] = zero_element;
    data.shrink_to_fit();
}



template <typename T>
bool Lattice3d<T>::randomize()
{
    #pragma omp critical
    #pragma optimize("", off)
    for (uint x = 0; x < Lx_; ++x)
    {
        for (uint y = 0; y < Ly_; ++y)
        {
            for (uint z = 0; z < Lz_; ++z)
            {
                T t = rng::get_random<T>();
                this->operator()(x, y, z) =  rng::get_random<T>();
            }
        }
    }
    #pragma optimize("", on)
    return true;
}

template <typename T>
bool Lattice3d<T>::set_constant(T const& value){
    for (uint x = 0; x < Lx_; ++x)
    {
        for (uint y = 0; y < Ly_; ++y)
        {
            for (uint z = 0; z < Lz_; ++z)
            {
                this->set(x, y, z, value);
            }
        }
    }
    return 0;
}

template <typename T>
Lattice3d<T> Lattice3d<T>::constant_lattice(
    uint Lx, uint Ly, uint Lz, T const &value)
{
    Lattice3d lattice(Lx, Ly, Lz);
    for (uint x = 0; x < Lx; ++x)
    {
        for (uint y = 0; y < Ly; ++y)
        {
            for (uint z = 0; z < Lz; ++z)
            {
                lattice.set(x, y, z, value);
            }
        }
    }
    return lattice;
}

template <typename T>
Lattice3d<T> Lattice3d<T>::random_lattice(uint Lx, uint Ly, uint Lz)
{
    Lattice3d lattice(Lx, Ly, Lz);
    for (uint x = 0; x < Lx; ++x)
    {
        for (uint y = 0; y < Ly; ++y)
        {
            for (uint z = 0; z < Lz; ++z)
            {
                lattice.set(x, y, z, rng::get_random<T>());
            }
        }
    }
    return lattice;
}

//          --- Lattic3d

Lattice3d<bool>::Lattice3d(uint Lx, uint Ly, uint Lz)
    : zero_element(true), Lx_(Lx), Ly_(Ly), Lz_(Lz),
      data(Lx_ * Ly_ * Lz_)
{
    dummy_element = zero_element;
    data.resize(Lx_ * Ly_ * Lz_);
    data.shrink_to_fit();
}

// size of the lattice in x-direction
uint Lattice3d<bool>::Lx() const { return Lx_; }
// size of the lattice in y-direction
uint Lattice3d<bool>::Ly() const { return Ly_; }

uint Lattice3d<bool>::Lz() const { return Lz_; }

BC Lattice3d<bool>::get_boundary_conditions() const { return bc; }

// set value of the BC::_0
void Lattice3d<bool>::set_zero_element(bool const &zero)
{
    zero_element = zero;
}

void Lattice3d<bool>::set_boundary_conditions(BC bc_) { bc = bc_; }
// acess operator const


void Lattice3d<bool>::set(int const &x, int const &y, int const &z,
                          bool const &v)
{
    switch (bc)
    {
    case BC::_0:
        if (x >= 0 && x < Lx_ &&
            y >= 0 && y < Ly_ &&
            z >= 0 && z < Lz_)
        {
            data.at(access(x, y, z)) = v;
        }
        break;
    // BC = periodic
    default:
        uint x_ = modulo(x, Lx_);
        uint y_ = modulo(y, Ly_);
        uint z_ = modulo(z, Lz_);
        data.at(access(x_, y_, z_)) = v;
        break;
    }
}

void Lattice3d<bool>::set(Index const &id, bool const &v)
{
    set(id[0], id[1], id[2], v);
}

bool Lattice3d<bool>::get(int const &x, int const &y, int const &z) const
{
    switch (bc)
    {
    case BC::_0:
        if (x >= 0 && x < Lx_ &&
            y >= 0 && y < Ly_ &&
            z >= 0 && z < Lz_)
        {
            return data.at(access(x, y, z));
        }
        return zero_element;
        break;
    // BC = periodic
    default:
        uint x_ = modulo(x, Lx_);
        uint y_ = modulo(y, Ly_);
        uint z_ = modulo(z, Lz_);
        return data.at(access(x_, y_, z_));
    }
}

bool Lattice3d<bool>::get(Index const &id) const
{
    return get(id[0],id[1],id[2]);
}

bool Lattice3d<bool>::randomize()
{
    for (uint x = 0; x < Lx_; ++x)
    {
        for (uint y = 0; y < Ly_; ++y)
        {
            for (uint z = 0; z < Lz_; ++z)
            {
                
                this->set(x, y, z, rng::rand_uniform() < 0.5);
            }
        }
    }
    return true;
}

bool Lattice3d<bool>::set_constant(bool const& value){
    for (uint x = 0; x < Lx_; ++x)
    {
        for (uint y = 0; y < Ly_; ++y)
        {
            for (uint z = 0; z < Lz_; ++z)
            {
                this->set(x, y, z, value);
            }
        }
    }
    return true;
}

Lattice3d<bool> Lattice3d<bool>::constant_lattice(
        uint Lx, uint Ly, uint Lz, bool const &value){
    Lattice3d<bool> lattice(Lx, Ly, Lz);
    for (uint x = 0; x < Lx; ++x)
    {
        for (uint y = 0; y < Ly; ++y)
        {
            for (uint z = 0; z < Lz; ++z)
            {
                lattice.set(x, y, z, value);
            }
        }
    }
    return lattice;
}

Lattice3d<bool> Lattice3d<bool>::random_lattice(uint Lx, uint Ly, uint Lz)
{
    Lattice3d lattice(Lx, Ly, Lz);
    for (uint x = 0; x < Lx; ++x)
    {
        for (uint y = 0; y < Ly; ++y)
        {
            for (uint z = 0; z < Lz; ++z)
            {
                bool random_bool = rng::rand_uniform() < 0.5;
                lattice.set(x, y, z, random_bool);
            }
        }
    }
    return lattice;
}

template class Lattice3d<SpinVector>;
template class Lattice3d<bool>;