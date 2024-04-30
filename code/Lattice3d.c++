#include <Lattice3d.h++>
/*
mathematical correct modulo

/ @brief
/ @param i int
/ @param u uint < 2³¹
/ @return i mod u
*/
uint modulo(int const &i, uint u)
{
    constexpr uint max_u = 1 << 31;
    // u = min(max_u, u); to expensive
    return (i % static_cast<int>(u) 
             + static_cast<int>(u)) % static_cast<int>(u);
}


// size of the lattice in x-direction
template <typename T>
uint Lattice3d<T>::Lx() const { return Lx_; }
    // size of the lattice in y-direction
template<typename T>
uint Lattice3d<T>::Ly() const { return Ly_; }
    
template<typename T>
uint Lattice3d<T>::Lz() const { return Lz_; }

    // get boundary condition
template <typename T>
BC Lattice3d<T>::get_boundary_conditions() const {return bc;}

// set value of the BC::_0
template <typename T>
void Lattice3d<T>::set_zero_element(T const& zero){
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
            return data[x * Ly_ * Lz_ + y * Lz_ + z];
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
        return data.at(x_ * Ly_ * Lz_ + y_ * Lz_ + z_);
        break;
    }
}



template <typename T>
T& Lattice3d<T>::operator()(int x, int y, int z)
{
    switch (bc)
    {
    case BC::_0:
        if (x >= 0 && x < Lx_ &&
            y >= 0 && y < Ly_ &&
            z >= 0 && z < Lz_)
        {
            return data[x * Ly_ * Lz_ + y * Lz_ + z];
        }
        else
        {
            dummy_element = zero_element;
            return dummy_element;
        }
        break;

    default:
        uint x_ = modulo(x, Lx_);
        uint y_ = modulo(y, Ly_);
        uint z_ = modulo(z, Lz_);
        return data[x_ * Ly_ * Lz_ + y_ * Lz_ + z_];
        ;
        break;
    }
}

template<>
bool& Lattice3d<bool>::operator()(int x, int y, int z){
    dummy_element = zero_element;
    return dummy_element;
}

template <typename T>
T& Lattice3d<T>::operator()(Index const& id){
    return this->operator()(id[0], id[1], id[2]);
}

template <typename T>
T Lattice3d<T>::operator()(Index const& id) const{
    return this->operator()(id[0], id[1], id[2]);
}

template <typename T>
void Lattice3d<T>::set( int const& x, int  const& y, int const& z, 
                        T const& v){
    switch (bc)
    {
    case BC::_0:
        if (x >= 0 && x < Lx_ &&
            y >= 0 && y < Ly_ &&
            z >= 0 && z < Lz_)
        {
            data[x * Ly_ * Lz_ + y * Lz_ + z] = v;
        }
        break;
    // BC = periodic
    default:
        uint x_ = modulo(x, Lx_);
        uint y_ = modulo(y, Ly_);
        uint z_ = modulo(z, Lz_);
        data[x_ * Ly_ * Lz_ + y_ * Lz_ + z_] = v;
        ;
        break;
    }
}

template <typename T>
void Lattice3d<T>::set(Index const& id, T const& v){
    set(id[0],id[1],id[2],v);
}

template <typename T>
T Lattice3d<T>::get(int const& x, int  const& y, int const& z) const{
    return this->operator()(x,y,z);
}

template <typename T>
T Lattice3d<T>::get(Index const& id) const{
    return this->operator()(id);
}

template <typename T>
Lattice3d<T>::Lattice3d(uint Lx, uint Ly, uint Lz)
        :   zero_element(0), Lx_(Lx), Ly_(Ly), Lz_(Lz), 
            data(Lx * Ly * Lz)
{
    data.resize(Lx * Ly * Lz);
    data.shrink_to_fit();
}

template<>
Lattice3d<bool>::Lattice3d(uint Lx, uint Ly, uint Lz)
        :   zero_element(true), Lx_(Lx), Ly_(Ly), Lz_(Lz), 
            data(Lx_ * Ly_ * Lz_ + 1)
{
    data.resize(Lx_ * Ly_ * Lz_ + 1);
    data.shrink_to_fit();
}

template <typename T>
bool Lattice3d<T>::randomize()
{
    for (uint x = 0; x < Lx_; ++x)
    {
        for (uint y = 0; y < Ly_; ++y)
        {
            for (uint z = 0; z < Lz_; ++z)
            {
                (*this)(x, y, z) = rng::get_random<T>();
            }
        }
    }
    return true;
}

template <typename T>
Lattice3d<T> Lattice3d<T>::constant_lattice(
        uint Lx, uint Ly, uint Lz,T const &value)
{
    Lattice3d lattice(Lx, Ly, Lz);
    for (uint x = 0; x < Lx; ++x)
    {
        for (uint y = 0; y < Ly; ++y)
        {
            for (uint z = 0; z < Lz; ++z)
            {
                lattice(x, y, z) = value;
            }
        }
    }
    return lattice;
}

template <typename T>
Lattice3d<T> Lattice3d<T>::random_lattice(uint Lx,uint Ly,uint Lz)
{
    Lattice3d lattice(Lx, Ly, Lz);
    for (uint x = 0; x < Lx; ++x)
    {
        for (uint y = 0; y < Ly; ++y)
        {
            for (uint z = 0; z < Lz; ++z)
            {
                lattice(x, y, z) = rng::get_random<T>();
            }
        }
    }
    return lattice;
}

template class Lattice3d<SpinVector>;
template class Lattice3d<bool>;