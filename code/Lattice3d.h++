#ifndef LATTICE3D_HPP
#define LATTICE3D_HPP
#include <Base.h++>
#include <Spin.h++>

// Boundary Conditions
enum BC{
    Dirichlet,
    Periodic
};


/*
mathematical correct modulo

/ @brief
/ @param i int
/ @param u uint < 2³¹
/ @return i mod u
*/
inline uint modulo(int const& i, uint u) {
    static constexpr uint max_u  = 1 << 31;
    // u = min(max_u, u); zo expensive
    return (i % static_cast<int>(u) + static_cast<int>(u)) 
            % static_cast<int>(u);
}


// Lattice Containder
/*

Acsess
T x = lattice<T>(x,y,z)

*/
template<class T>
class Lattice3d{
private:
    // datavector
    vector<T> data;
    // dimentions
    uint Lx,Ly,Lz;
public:
    BC bc = BC::Periodic;
    // acess operator const
    inline T operator()(int x, int y, int z) const{
        switch (bc)
        {
        case BC::Dirichlet:
            if(     x >= 0 && x < Lx &&
                    y >= 0 && y < Ly &&
                    z >= 0 && z < Lz){
                return data.at(x*Ly*Lz + y*Lz + z);
            }
            else{
                return T(0);
            }
            break;
        
        default:
            uint x_ = modulo(x,Lx);
            uint y_ = modulo(y,Ly);
            uint z_ = modulo(x,Lx);
            return data.at(x_*Ly*Lz + y_*Lz + z_);
            break;
        }
    }
    // acess operator refrence
    inline T& operator()(int x, int y, int z){
        switch (bc)
        {
        case BC::Dirichlet:
            if(     x >= 0 && x < Lx &&
                    y >= 0 && y < Ly &&
                    z >= 0 && z < Lz){
                return data.at(x*Ly*Lz + y*Lz + z);
            }
            else{
                return data.at(Lx*Ly*Lz) = T(0);
            }
            break;
        
        default:
            uint x_ = modulo(x,Lx);
            uint y_ = modulo(y,Ly);
            uint z_ = modulo(x,Lx);
            return data.at(x_*Ly*Lz + y_*Lz + z_);
            break;
        }
    }



    Lattice3d(  uint Lx, uint Ly, uint Lz, uint seed=42)
                : Lx(Lx), Ly(Ly), Lz(Lz), data(Lx*Ly*Lz+1){
        srand(seed);
        for(uint x = 0; x < Lx; ++x){
            for(uint y = 0; y < Ly; ++y){
                for(uint z = 0; z < Lz; ++z){
                    (*this)(x,y,z) = T();
                }
            }
        }
    }
    Lattice3d(Lattice3d& other) = default;
    Lattice3d(Lattice3d const& other) = default;

    bool regenerate(uint seed=42){
        srand(seed);
        for(uint x = 0; x < Lx; ++x){
            for(uint y = 0; y < Ly; ++y){
                for(uint z = 0; z < Lz; ++z){
                    (*this)(x,y,z) = T();
                }
            }
        }
        return true;
    }

    int memory_size()const{
        return data.size()*sizeof(T);
    }
};

using Lattice = Lattice3d<Spin>;
#endif