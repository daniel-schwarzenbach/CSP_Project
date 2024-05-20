#include <Lattice3D.h++>

// A function that finds out if a uint is a power of 2 and if it isn't
// returns the next lower power of two
inline uint power_of_2_or_lower(uint n)
{
    // Check if the number is zero
    if (n == 0)
    {
        // Return zero
        return 0;
    }
    // Check if the number is a power of 2 by using the bitwise AND
    // operation
    // If n & (n - 1) is zero, then n has a single set bit
    if ((n & (n - 1)) == 0)
    {
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
    return (i % static_cast<int>(u) + static_cast<int>(u)) 
            % static_cast<int>(u);
}

/*
function to access the undelying Lattice Vector

/ @brief
/ @param x: position at x
/ @param y: position at y
/ @param z: position at z
/ @param Lx: X dimension
/ @param Ly: Y dimension
/ @param Lz: Z dimension
/ @param bc: Boundary condition
/ @return id for data array
*/
inline uint get_id(int const &x, int const &y, int const &z,
                   uint const &Lx, uint const &Ly, uint const &Lz,
                   BC const &bc, u64 const &fullSize)
{
    // check if the 
    if (x >= 0 && x < Lx &&
        y >= 0 && y < Ly &&
        z >= 0 && z < Lz)
    {
        return x * Ly * Lz + y * Lz + z;
    }
    // else
    switch (bc)
    {
    case BC::Dirichlet:
        return fullSize;
    default:
        // way more effitient modolo, 
        // but requires that Lx,Ly,Lz are powers of 2
        #ifndef WITHOUT_POW2
        uint x_ = x & (Lx - 1);
        uint y_ = y & (Ly - 1);
        uint z_ = z & (Lz - 1);
        #else
        uint x_ = modulo(x,Lx);
        uint y_ = modulo(y,Ly);
        uint z_ = modulo(z,Lz);
        #endif
        return x_ * Ly * Lz + y_ * Lz + z_;
    }
}

/*
function to access the undelying Lattice Vector for refrences for
dirichlet BC

/ @brief
/ @param x: position at x
/ @param y: position at y
/ @param z: position at z
/ @param Lx: X dimension
/ @param Ly: Y dimension
/ @param Lz: Z dimension
/ @param bc: Boundary condition
/ @param data: data array: important for dirichlet
/ @return id for data array
*/
template<typename T>
inline uint get_id_ref(int const &x, int const &y, int const &z,
                   uint const &Lx, uint const &Ly, uint const &Lz,
                   BC const &bc, u64 const &fullSize, 
                   Array<T>& data)
{
    // check if the 
    if (x >= 0 && x < Lx &&
        y >= 0 && y < Ly &&
        z >= 0 && z < Lz)
    {
        return x * Ly * Lz + y * Lz + z;
    }
    // else
    switch (bc)
    {
    case BC::Dirichlet:
        // ensure that rhe boundry stays correct with refrences
        data[fullSize+1] = data[fullSize];
        return fullSize+1;
    default:
        // way more effitient modolo, 
        // but requires that Lx,Ly,Lz are powers of 2
        #ifndef WITHOUT_POW2
        uint x_ = x & (Lx - 1);
        uint y_ = y & (Ly - 1);
        uint z_ = z & (Lz - 1);
        #else
        uint x_ = modulo(x,Lx);
        uint y_ = modulo(y,Ly);
        uint z_ = modulo(z,Lz);
        #endif
        return x_ * Ly * Lz + y_ * Lz + z_;
    }
}


//========================= Lattic3D =================================

// return raw data
template <typename T>
Array<T> &Lattice3D<T>::get_raw_data()
{
    return data;
}
// return raw data id
template <typename T>
uint Lattice3D<T>::get_raw_id(
    int const &x, int const &y, int const &z)
{
    return get_id_ref<T>(x, y, z, Lx_, Ly_, Lz_, bc, fullSize, data);
}

// return raw data id
template <typename T>
uint Lattice3D<T>::get_raw_id(Index const &index)
{
    return get_id_ref<T>(index[0], index[1], index[2], Lx_, Ly_, Lz_, 
            bc, fullSize, data);
}

// size of the lattice in x-direction
template <typename T>
uint Lattice3D<T>::Lx() const { return Lx_; }
// size of the lattice in y-direction
template <typename T>
uint Lattice3D<T>::Ly() const { return Ly_; }
// size of the lattice in z-direction
template <typename T>
uint Lattice3D<T>::Lz() const { return Lz_; }

// get boundary condition
template <typename T>
BC Lattice3D<T>::get_boundary_conditions() const { return bc; }

// set value of the BC::Dirichlet
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
    return data.at(get_id(x, y, z, Lx_, Ly_, Lz_, bc, fullSize));
}

// acess operator ref
template <typename T>
T &Lattice3D<T>::operator()(Index const &id)
{
    return data.at(get_id_ref<T>(id[0], id[1], id[2], Lx_, Ly_, Lz_, 
            bc, fullSize, data));
}


// access operator const
template <typename T>
T Lattice3D<T>::operator()(Index const &id) const
{
    return data.at(get_id(id[0], id[1], id[2], Lx_, Ly_, Lz_, bc,
            fullSize));
}

// acces operator ref
template <typename T>
T &Lattice3D<T>::operator()(int x, int y, int z)
{
    return data.at(get_id_ref<T>(x, y, z, Lx_, Ly_, Lz_, bc, fullSize, 
            data));
}

template <typename T>
Lattice3D<T>::Lattice3D(uint Lx, uint Ly, uint Lz)
    : Lx_(0), Ly_(0), Lz_(0),
      data(0), fullSize(0)
{
    // ensure that the dimensions{Lx,Ly,Lz} are all powers of 2
    #ifndef WITHOUT_POW2
    Lx_ = power_of_2_or_lower(Lx);
    Ly_ = power_of_2_or_lower(Ly);
    Lz_ = power_of_2_or_lower(Lz);
    if (Lx_ != Lx || Lx_ != Lx || Lx_ != Lx)
    {
        cout << WARNING << " lattice has been set to 2ˣ :"
             << "{" << Lx_ << "," << Ly_ << "," << Lz_ << "}" << endl;
    }
    #endif
    // callculate the full size
    fullSize = Lx_ * Ly_ * Lz_;
    // ensure that the data has the correct size
    data.resize(fullSize + 2); // +2 are the dirichlet elements
    // set the +1 to 0
    data[fullSize] = T(0); // true boundry
    data[fullSize+1] = data[fullSize]; // boundry value for refrences
    // ensure that we do not use do much data, keep the data compact
    data.shrink_to_fit();
}

template <typename T>
bool Lattice3D<T>::randomize()
{
// We need to turn off optimizations so that we get always the same
// Lattice for the same seed
#pragma optimize("", off)
    // set all values random
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
// turn on optimizations back on
#pragma optimize("", on)
    return true;
}

// set the entire lattice to a specific variable
template <typename T>
bool Lattice3D<T>::set_constant(T const &value)
{
    // set all values constant
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

// returns a constant Lattice
template <typename T>
Lattice3D<T> Lattice3D<T>::constant_lattice(
    uint Lx, uint Ly, uint Lz, T const &value)
{
    // init lattice
    Lattice3D lattice(Lx, Ly, Lz);
    // set all values constant
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

// returns a random lattice
template <typename T>
Lattice3D<T> Lattice3D<T>::random_lattice(uint Lx, uint Ly, uint Lz)
{
    // init lattice
    Lattice3D lattice(Lx, Ly, Lz);
    // set random values
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
    #ifndef WITHOUT_POW2
    // ensure that the dimensions{Lx,Ly,Lz} are all powers of 2
    Lx_ = power_of_2_or_lower(Lx);
    Ly_ = power_of_2_or_lower(Ly);
    Lz_ = power_of_2_or_lower(Lz);
    if (Lx_ != Lx || Lx_ != Lx || Lx_ != Lx)
    {
        cout << WARNING << " lattice has been set to 2ˣ :"
             << "{" << Lx_ << "," << Ly_ << "," << Lz_ << "}" << endl;
    }
    #endif
    // callculate the full size
    fullSize = Lx_ * Ly_ * Lz_; // +1 is the dirichlet boundry element
    data.resize(fullSize + 2);
    // set the +2 to true so that we do not go outside of the lattice
    data[fullSize] = true; // true boundary
    data[fullSize + 1] = true; // boundry element for refrences
    // ensure that we do not use do much data, keep the data compact
    data.shrink_to_fit();
}

// return raw data id
uint Lattice3D<bool>::get_raw_id(int const &x, int const &y,
                                 int const &z)
{
    return get_id_ref<bool>(x, y, z, Lx_, Ly_, Lz_, bc, fullSize, 
            data);
}

// return raw data id
uint Lattice3D<bool>::get_raw_id(Index const& id)
{
    return get_id_ref<bool>(id[0], id[1], id[2], Lx_, Ly_, Lz_, bc, fullSize, 
            data);
}

// return raw data
Array<bool> &Lattice3D<bool>::get_raw_data()
{
    return data;
}

// size of the lattice in x-direction
uint Lattice3D<bool>::Lx() const { return Lx_; }
// size of the lattice in y-direction
uint Lattice3D<bool>::Ly() const { return Ly_; }
// size of the lattice in z-direction
uint Lattice3D<bool>::Lz() const { return Lz_; }

// return boundary condition
BC Lattice3D<bool>::get_boundary_conditions() const { return bc; }

// set value of the BC::Dirichlet
void Lattice3D<bool>::set_zero_element(bool const &zero)
{
    data[fullSize] = zero;
}

// change boundry condition
void Lattice3D<bool>::set_boundary_conditions(BC bc_) { bc = bc_; }

// change the value a x,y,z
void Lattice3D<bool>::set(int const &x, int const &y, int const &z,
                          bool const &v)
{
    data[get_id_ref<bool>(x, y, z, Lx_, Ly_, Lz_, bc, fullSize, 
            data)] = v;
}

// change the value at index
void Lattice3D<bool>::set(Index const &id, bool const &v)
{
    data[get_id_ref<bool>(id[0], id[1], id[2], Lx_, Ly_, Lz_, bc, 
            fullSize, data)] = v;
}

// return the value at x,y,z
bool Lattice3D<bool>::get(int const &x, int const &y, int const &z)
    const
{
    return data[get_id(x, y, z, Lx_, Ly_, Lz_, bc, fullSize)];
}

// return the value at index
bool Lattice3D<bool>::get(Index const &id) const
{
    return data[get_id(id[0], id[1], id[2], Lx_, Ly_, Lz_, bc, 
            fullSize)];
}

// randomize the lattice
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

// set lattice to a const value
bool Lattice3D<bool>::set_constant(bool const &value)
{
    // set all values to the specified value
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

// return a constant lattice
Lattice3D<bool> Lattice3D<bool>::constant_lattice(
    uint Lx, uint Ly, uint Lz, bool const &value)
{
    // init lattice
    Lattice3D<bool> lattice(Lx, Ly, Lz);
    // set all values to the specified value
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

// return a new random lattice
Lattice3D<bool> Lattice3D<bool>::random_lattice(uint Lx, uint Ly, 
                                                uint Lz)
{
    // init lattice
    Lattice3D lattice(Lx, Ly, Lz);
    // set random bools
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




// ====================== compile lattices ===========================
template class Lattice3D<Spin>;
template class Lattice3D<u8>;
template class Lattice3D<bool>;