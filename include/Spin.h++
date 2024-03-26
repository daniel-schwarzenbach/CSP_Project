#pragma once

#include <Base.h++>
#include <immintrin.h> // Include SIMD intrinsics header file 
                       // for vectorization


// // uses vectorization
// class SpinFast{
// private:
//     flt* data;
// public:
    
//     SpinFast(){
//         data = (flt*) malloc(sizeof(flt)*3);
//         flt φ = randflt() * ₂π;
//         flt ϑ = randflt() * π;
//         data[0] = sin(φ)*cos(ϑ);
//         data[1] = cos(φ)*cos(ϑ);
//         data[2] = sin(ϑ);
//     }
//     SpinFast(SpinFast const& other){
//         data = (flt*) malloc(sizeof(flt)*3);
//         data[0] = other.data[0];
//         data[1] = other.data[1];
//         data[2] = other.data[2];
//     }
//     // SpinFast(flt x, flt y, flt z){}
//     ~SpinFast(){if(data != NULL) free(data);}


//     SpinFast& operator=(SpinFast const& other){
//         data[0] = other.data[0];
//         data[1] = other.data[1];
//         data[2] = other.data[2];
//         return *this;
//     }
//     // scalar product
//     flt operator|(SpinFast const& other){
//         __m128 v1 = _mm_loadu_ps(this->data);
//         __m128 v2 = _mm_loadu_ps(other.data);

//         // Perform dot product using SIMD multiplication and addition
//         __m128 result = _mm_dp_ps(v1, v2, 0b01110001);

//         // Extract the scalar product result from the SIMD vector
//         float resultScalar;
//         _mm_store_ss(&resultScalar, result);
//         return resultScalar;
//     }
// };

class Spin3d{
public:
    flt x=1,y=0,z=0;
    Spin3d(){
        flt φ = randflt() * ₂π;
        flt ϑ = randflt() * π - π_2;
        x = sin(φ)*cos(ϑ);
        y = cos(φ)*cos(ϑ);
        z = sin(ϑ);
    }
    Spin3d(flt x, flt y=0, flt z=0):x(x), y(y), z(z) {}
    Spin3d& operator=(Spin3d const& other){
        this->x = other.x;
        this->y = other.y;
        this->z = other.z;
        return *this;
    };
    // scalar product
    flt operator|(Spin3d const& other){
        return this->x*other.x + this->y*other.y + this->z*other.z;
    }
    bool operator==(Spin3d const& other){
        return  (abs(this->x==other.x)<1e-3) && 
                (abs(this->y==other.y)<1e-3) &&
                (abs(this->z==other.z)<1e-3);
    }
    friend std::ostream& operator<<(std::ostream& os, 
                                    Spin3d const& s){
        os << "{" << s.x << ", " << s.y << ", " << s.z << "}";
        return os;
    }
};

class SpinPol{
public:
    flt φ = 0, ϑ = 0;
    SpinPol(){
        flt φ = randflt() * ₂π;
        flt ϑ = randflt() * π - π_2;
    }
    SpinPol(flt φ, flt ϑ=0): φ(φ), ϑ(ϑ){}
    SpinPol& operator=(SpinPol const& other){
        this->φ = other.φ;
        this->ϑ = other.ϑ;
        return *this;
    };
    // scalar product
    flt operator|(SpinPol const& other){
        return  cos(this->ϑ) * cos(other.ϑ) * cos(this->φ - other.φ)
                + sin(this->ϑ) * sin(other.ϑ);
    }
};

using Spin = Spin3d;