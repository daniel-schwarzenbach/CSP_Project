#ifndef __SPIN_H__
#define __SPIN_H__



#include <Base.h++>

/*
compute: efficient, 0.42 ns
memory: inefficient, sizeof(flt)*3 ~ (float)96bit
*/
class SpinXYZ
{
private:
    flt x_ = 1, y_ = 0, z_ = 0;
public:
    inline flt x() const { return x_;}
    inline flt y() const { return y_;}
    inline flt z() const { return z_;}
    inline flt θ() const { return acos(z_); }
    inline flt ϕ() const { return atan2(y_, x_); }
    // generate random spin
    SpinXYZ()
    {
        flt φ = randflt() * ₂π;
        flt ϑ = randflt() * π - π_2;
        x_ = sin(φ) * sin(ϑ);
        y_ = cos(φ) * sin(ϑ);
        z_ = cos(ϑ);
    }
    SpinXYZ(flt zero) : x_(0), y_(0), z_(0) {}
    inline SpinXYZ &operator=(SpinXYZ const &other)
    {
        this->x_ = other.x_;
        this->y_ = other.y_;
        this->z_ = other.z_;
        return *this;
    };
    // scalar product
    inline flt operator|(SpinXYZ const &other)
    {
        return      this->x_ * other.x_ + this->y_ * other.y_ 
                +   this->z_ * other.z_;
    }
    // compareration operator
    bool operator==(SpinXYZ const &other)
    {
        return (abs(this->x_ == other.x_) < 1e-3) &&
               (abs(this->y_ == other.y_) < 1e-3) &&
               (abs(this->z_ == other.z_) < 1e-3);
    }
    // output operator
    friend std::ostream &operator<<(std::ostream &os,
                                    SpinXYZ const &s)
    {
        os << "{" << s.x_ << ", " << s.y_ << ", " << s.z_ << "}";
        return os;
    }
    
    SpinXYZ from_xyz(flt x, flt y, flt z)
    {
        SpinXYZ s;
        s.x_ = x;
        s.y_ = y;
        s.z_ = z;
        return s;
    }
    SpinXYZ from_ϕθ(flt ϕ, flt θ)
    {
        SpinXYZ s;
        s.x_ = sin(θ) * cos(ϕ);
        s.y_ = sin(θ) * sin(ϕ);
        s.z_ = cos(θ);
        return s;
    }
};


// @constants for Spinϕθ:
constexpr flt spinpolθFac = π / 0xfe;
constexpr flt spinpolϕFac = ₂π / 0xff;
/*
x = sin(θ) ⋅ cos(ϕ)
y = sin(θ) ⋅ sin(ϕ)
z = sin(θ)

θ_byte = 0xff  =>  zero Spin

compute: inefficient, 20.77 ns, 50x slower than SpinXYZ
memory: efficient, 16Bit, 1/6 of SpinXYZ
*/
class Spinϕθ
{
private:
    U8 ϕ_byte = 0;
    U8 θ_byte = 0;

public:
    inline flt ϕ() const { return ϕ_byte * spinpolϕFac; }
    inline flt θ() const { return θ_byte * spinpolθFac; }
    inline flt x() const { return sin(θ()) * cos(ϕ()); }
    inline flt y() const { return sin(θ()) * sin(ϕ()); }
    inline flt z() const { return cos(θ()); }
    // generate random spin
    Spinϕθ()
    {
        ϕ_byte = round(randflt() * 0xff);
        θ_byte = round(randflt() * 0xfe);
    }
    // to initialize a zero Spin
    Spinϕθ(flt zero)
    {
        if(zero == 0){
            ϕ_byte = 0;
            θ_byte = 0xff;
        } else{
        ϕ_byte = 0;
        θ_byte = 0;
        }
    }

    inline Spinϕθ &operator=(Spinϕθ const &other)
    {
        this->ϕ_byte = other.ϕ_byte;
        this->θ_byte = other.θ_byte;
        return *this;
    };

    // scalar product (1,θ₁,ϕ₁) ⋅ (1,θ₂,ϕ₂)
    // - returns 0 if any θ=0xff
    inline flt operator|(Spinϕθ const &other)
    {
        flt θ₁ = this->θ();
        flt θ₂ = other.θ();
        if(θ₁ == 0xff || θ₂ == 0xff) return 0; // => zero Spin
        // Wolfram Alpha told me that
        return    sin(θ₁) * sin(θ₂) * cos(this->ϕ() - other.ϕ()) 
                + cos(θ₁) * cos(θ₂);
    }
    // output operator
    friend std::ostream &operator<<(std::ostream &os,
                                    Spinϕθ const &s)
    {
        os << "{" << s.x() << ", " << s.y() << ", " << s.z() << "}";
        return os;
    }
    // compare operator
    bool operator==(Spinϕθ const &other)
    {
        return  this->ϕ_byte == other.ϕ_byte && 
                this->θ_byte == other.θ_byte;
    }

    Spinϕθ from_xyz(flt x, flt y, flt z)
    {
        Spinϕθ s;
        s.θ_byte = round(acos(z) / spinpolθFac);
        s.ϕ_byte = round(atan2(y, x) / spinpolϕFac);
        return s;
    }
    Spinϕθ from_ϕθ(flt ϕ, flt θ)
    {
        Spinϕθ s;
        s.θ_byte = round(θ / spinpolθFac);
        s.ϕ_byte = round(ϕ / spinpolϕFac);
        return s;
    }
};

#endif // __SPIN_H__