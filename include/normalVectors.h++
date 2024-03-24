#include <Base.h++>
#include <immintrin.h> // Include SIMD intrinsics header file 
                       // for vectorization

class NormVec{
    // scalar product
    virtual flt operator|(NormVec const& other){return 0.;};
    
};

// uses vectorization
class NormFast : public NormVec{
private:
    flt* data;
public:
    
    NormFast(){
        data = (flt*) malloc(sizeof(flt)*3);
        flt φ = randflt() * ₂π;
        flt ϑ = randflt() * π;
        data[0] = sin(φ)*cos(ϑ);
        data[1] = cos(φ)*cos(ϑ);
        data[2] = sin(ϑ);
    }
    NormFast(NormFast const& other){
        data = (flt*) malloc(sizeof(flt)*3);
        data[0] = other.data[0];
        data[1] = other.data[1];
        data[2] = other.data[2];
    }
    // NormFast(flt x, flt y, flt z){}
    ~NormFast(){if(data != NULL) free(data);}


    NormFast& operator=(NormFast const& other){
        data[0] = other.data[0];
        data[1] = other.data[1];
        data[2] = other.data[2];
        return *this;
    }
    // scalar product
    flt operator|(NormFast const& other){
        __m128 v1 = _mm_loadu_ps(this->data);
        __m128 v2 = _mm_loadu_ps(other.data);

        // Perform dot product using SIMD multiplication and addition
        __m128 result = _mm_dp_ps(v1, v2, 0b01110001);

        // Extract the scalar product result from the SIMD vector
        float resultScalar;
        _mm_store_ss(&resultScalar, result);
        return resultScalar;
    }
};

class Norm3d : public NormVec{
public:
    flt x=1,y=0,z=0;
    Norm3d(){
        flt φ = randflt() * ₂π;
        flt ϑ = randflt() * π;
        x = sin(φ)*cos(ϑ);
        y = cos(φ)*cos(ϑ);
        z = sin(ϑ);
    }
    Norm3d(flt x, flt y, flt z):x(x), y(y), z(z) {}
    Norm3d& operator=(Norm3d const& other){
        this->x = other.x;
        this->y = other.y;
        this->z = other.z;
        return *this;
    };
    // scalar product
    flt operator|(Norm3d const& other){
        return this->x*other.x + this->y*other.y + this->z*other.z;
    }
};

class NormPol : public NormVec{
public:
    flt φ = 0, ϑ = 0;
    NormPol(){
        flt φ = randflt() * ₂π;
        flt ϑ = randflt() * π;
    }
    NormPol(flt φ, flt ϑ): φ(φ), ϑ(ϑ){}
    NormPol& operator=(NormPol const& other){
        this->φ = other.φ;
        this->ϑ = other.ϑ;
        return *this;
    };
    // scalar product
    flt operator|(NormPol const& other){
        return  cos(this->ϑ) * cos(other.ϑ) * cos(this->φ - other.φ)
                + sin(this->ϑ) * sin(other.ϑ);
    }
};