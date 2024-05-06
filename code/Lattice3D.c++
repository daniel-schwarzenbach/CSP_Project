#include <Lattice3D.h++>


// A function that finds out if a uint is a power of 2 and if it isn't 
// returns the next lower power of two
inline uint power_of_2_or_lower(uint n) {
    // Check if the number is zero
    if (n == 0) {
        // Return zero
        return 0;
    }
    // Check if the number is a power of 2 by using the bitwise AND 
    // operation
    // If n & (n - 1) is zero, then n has a single set bit
    if ((n & (n - 1)) == 0) {
        // Return the number itself
        return n;
    }
    // If the number is not a power of 2, clear the rightmost set bit 
    // by using the bitwise AND operation
    // This will give us the next lower power of 2
    return n & (n - 1);
}

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

/*
function to access the undelying Lattice Vector

/ @brief
/ @param x:
/ @param y:
/ @param z:
/ @param Lx:
/ @param Ly:
/ @param Lz:
/ @param bc: Boundary condition
/ @return id for data array
*/
inline uint get_id(int const& x, int const& y, int const& z, 
                  uint const& Lx, uint const&  Ly, uint const& Lz,
                  BC const& bc, u64 const& fullSize) {
    if (x >= 0 && x < Lx &&
        y >= 0 && y < Ly &&
        z >= 0 && z < Lz)
    {
        return x*Ly*Lz  +  y*Lz  +  z;
    }
    // else
    switch (bc)
    {
    case BC::_0:
        return fullSize;
    default:
        // uint x_ = modulo(x, Lx);
        // uint y_ = modulo(y, Ly);
        // uint z_ = modulo(z, Lz);
        // optimization for all L's = 2ˣ
        uint x_ = x & (Lx - 1);
        uint y_ = y & (Ly - 1);
        uint z_ = z & (Lz - 1);
        return x_*Ly*Lz  +  y_*Lz  +  z_;
        
    }
}


// return raw data
template <typename T>
Array<T> &Lattice3D<T>::get_raw_data()
{
    return data;
}
// return raw data id
template <typename T>
uint Lattice3D<T>::get_raw_id(
    int const &x, int const &y, int const &z) const
{
    return get_id(x,y,z,Lx_,Ly_,Lz_,bc,fullSize);
}

// return raw data id
template <typename T>
uint Lattice3D<T>::get_raw_id(Index const &index) const
{
    return get_id(index[0],index[1],index[2],Lx_,Ly_,Lz_,bc,fullSize);
}

// size of the lattice in x-direction
template <typename T>
uint Lattice3D<T>::Lx() const { return Lx_; }
// size of the lattice in y-direction
template <typename T>
uint Lattice3D<T>::Ly() const { return Ly_; }

template <typename T>
uint Lattice3D<T>::Lz() const { return Lz_; }

// get boundary condition
template <typename T>
BC Lattice3D<T>::get_boundary_conditions() const { return bc; }

// set value of the BC::_0
template <typename T>
void Lattice3D<T>::set_zero_element(T const &zero)
{
    data[fullSize] = zero;
}

template <typename T>
void Lattice3D<T>::set_boundary_conditions(BC bc_) { bc = bc_; }
// acess operator const

template <typename T>
T Lattice3D<T>::operator()(int x, int y, int z) const
{
    return data.at(get_id(x,y,z,Lx_,Ly_,Lz_,bc,fullSize));
}

template <typename T>
T &Lattice3D<T>::operator()(Index const &id)
{
    return data.at(get_id(id[0],id[1],id[2],Lx_,Ly_,Lz_,bc,fullSize));
}

template <typename T>
T Lattice3D<T>::operator()(Index const &id) const
{
    return data.at(get_id(id[0],id[1],id[2],Lx_,Ly_,Lz_,bc,fullSize));
}

template <typename T>
T &Lattice3D<T>::operator()(int x, int y, int z)
{
    return data.at(get_id(x,y,z,Lx_,Ly_,Lz_,bc,fullSize));
}


template <typename T>
Lattice3D<T>::Lattice3D(uint Lx, uint Ly, uint Lz)
    : Lx_(0), Ly_(0), Lz_(0),
      data(0), fullSize(0)
{
    Lx_ = power_of_2_or_lower(Lx);
    Ly_ = power_of_2_or_lower(Ly);
    Lz_ = power_of_2_or_lower(Lz);
    if(Lx_ != Lx || Lx_ != Lx || Lx_ != Lx){
        cout << WARNING << " lattice has been set to 2ˣ :" 
             << "{" << Lx_ << "," << Ly_ << "," << Lz_ << "}" << endl;
    }
    fullSize = Lx_*Ly_*Lz_;
    data.resize(fullSize + 1);
    data[fullSize] = 0;
    data.shrink_to_fit();
}

template <typename T>
bool Lattice3D<T>::randomize()
{
#pragma optimize("", off)
    for (uint x = 0; x < Lx_; ++x)
    {
        for (uint y = 0; y < Ly_; ++y)
        {
            for (uint z = 0; z < Lz_; ++z)
            {
                this->operator()(x, y, z) = rng::get_random<T>();
            }
        }
    }
#pragma optimize("", on)
    return true;
}

template <typename T>
bool Lattice3D<T>::set_constant(T const &value)
{
    for (uint x = 0; x < Lx_; ++x)
    {
        for (uint y = 0; y < Ly_; ++y)
        {
            for (uint z = 0; z < Lz_; ++z)
            {
                this->operator()(x, y, z) = value;
            }
        }
    }
    return 0;
}

template <typename T>
Lattice3D<T> Lattice3D<T>::constant_lattice(
    uint Lx, uint Ly, uint Lz, T const &value)
{
    Lattice3D lattice(Lx, Ly, Lz);
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
Lattice3D<T> Lattice3D<T>::random_lattice(uint Lx, uint Ly, uint Lz)
{
    Lattice3D lattice(Lx, Ly, Lz);
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




//========================= Lattic3D<bool>===========================

Lattice3D<bool>::Lattice3D(uint Lx, uint Ly, uint Lz)
    : Lx_(0), Ly_(0), Lz_(0),
      data(0), fullSize(0)
{
    Lx_ = power_of_2_or_lower(Lx);
    Ly_ = power_of_2_or_lower(Ly);
    Lz_ = power_of_2_or_lower(Lz);
    if(Lx_ != Lx || Lx_ != Lx || Lx_ != Lx){
        cout << WARNING << " lattice has been set to 2ˣ :" 
             << "{" << Lx_ << "," << Ly_ << "," << Lz_ << "}" << endl;
    }
    fullSize = Lx_*Ly_*Lz_;
    data.resize(fullSize + 1);
    data[fullSize] = true;
    data.shrink_to_fit();
}

// return raw data id
uint Lattice3D<bool>::get_raw_id(int const &x, int const &y, 
                                int const &z) const
{
    return get_id(x,y,z, Lx_,Ly_,Lz_,bc,fullSize);
}

// size of the lattice in x-direction
uint Lattice3D<bool>::Lx() const { return Lx_; }
// size of the lattice in y-direction
uint Lattice3D<bool>::Ly() const { return Ly_; }

uint Lattice3D<bool>::Lz() const { return Lz_; }

BC Lattice3D<bool>::get_boundary_conditions() const { return bc; }

// set value of the BC::_0
void Lattice3D<bool>::set_zero_element(bool const &zero)
{
    data[fullSize] = zero;
}

void Lattice3D<bool>::set_boundary_conditions(BC bc_) { bc = bc_; }
// acess operator const

void Lattice3D<bool>::set(int const &x, int const &y, int const &z,
                          bool const &v)
{
    data[get_id(x,y,z, Lx_,Ly_,Lz_,bc,fullSize)] = v;
}

void Lattice3D<bool>::set(Index const &id, bool const &v)
{
    data[get_id(id[0], id[1], id[2],Lx_,Ly_,Lz_,bc,fullSize)] = v;
}

bool Lattice3D<bool>::get(int const &x, int const &y, int const &z) 
        const
{
    return data[get_id(x,y,z, Lx_,Ly_,Lz_,bc,fullSize)];
}

bool Lattice3D<bool>::get(Index const &id) const
{
    return data[get_id(id[0], id[1], id[2],Lx_,Ly_,Lz_,bc,fullSize)];
}

bool Lattice3D<bool>::randomize()
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

bool Lattice3D<bool>::set_constant(bool const &value)
{
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

Lattice3D<bool> Lattice3D<bool>::constant_lattice(
    uint Lx, uint Ly, uint Lz, bool const &value)
{
    Lattice3D<bool> lattice(Lx, Ly, Lz);
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

Lattice3D<bool> Lattice3D<bool>::random_lattice(uint Lx, uint Ly, uint Lz)
{
    Lattice3D lattice(Lx, Ly, Lz);
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

template class Lattice3D<SpinVector>;
template class Lattice3D<bool>;