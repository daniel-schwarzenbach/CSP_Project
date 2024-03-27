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

// compute: efficient, 0.42 ns
// memory: inefficient, sizeof(flt)*3 ~ (float)96bit
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
    inline flt operator|(Spin3d const& other){
        return this->x*other.x + this->y*other.y + this->z*other.z;
    }
    // compareration operator
    bool operator==(Spin3d const& other){
        return  (abs(this->x==other.x)<1e-3) && 
                (abs(this->y==other.y)<1e-3) &&
                (abs(this->z==other.z)<1e-3);
    }
    // output operator
    friend std::ostream& operator<<(std::ostream& os, 
                                    Spin3d const& s){
        os << "{" << s.x << ", " << s.y << ", " << s.z << "}";
        return os;
    }
};

// compute: inefficient, 20.77 ns, 50x slower than Spin3d
// memory: efficient, 16Bit, 1/6 of Spin3d
constexpr flt spinpolθFac = π / 0xff;
constexpr flt spinpolϕFac = ₂π / 0xff;

class SpinPol{
private:
    U8 ϕ_byte = 0, θ_byte = 0;
public:
    inline flt ϕ()const{return ϕ_byte * spinpolϕFac;}
    inline flt θ()const{return θ_byte * spinpolθFac;}
    SpinPol(){
        ϕ_byte = round(randflt() * 0xff);
        θ_byte = round(randflt() * 0xff);
    }
    SpinPol(flt φ, flt ϑ=0){
        ϕ_byte = round(φ / spinpolϕFac);
        θ_byte = round(ϑ / spinpolθFac);
    }
    SpinPol& operator=(SpinPol const& other){
        this->ϕ_byte = other.ϕ_byte;
        this->θ_byte = other.θ_byte;
        return *this;
    };

    // scalar product (1,θ₁,ϕ₁) ⋅ (1,θ₂,ϕ₂)
    inline flt operator|(SpinPol const& other){
        flt θ₁ = this->θ();
        flt θ₂ = other.θ();
        // Wolfram Alpha told me that
        return  sin(θ₁) * sin(θ₂) * cos(this->ϕ() - other.ϕ())
                + cos(θ₁) * cos(θ₂);
    }
};



using Spin = Spin3d;

