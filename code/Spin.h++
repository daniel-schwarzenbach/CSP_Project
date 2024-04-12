#ifndef __SPIN_H__
#define __SPIN_H__

#include <Base.h++>

/*
[x, y, z]†
compute: efficient, 0.42 ns
memory: inefficient, sizeof(flt)*3 ~ (float)96bit
*/
class SpinCartesian
{
private:
    flt x_ = 1, y_ = 0, z_ = 0;

public:
    flt x() const;
    flt y() const;
    flt z() const;
    flt θ() const;
    flt ϕ() const;
    // generate random spin
    SpinCartesian();
    SpinCartesian(flt zero);
    SpinCartesian &operator=(SpinCartesian const &other);
    // scalar product
    flt operator|(SpinCartesian const &other);
    // compareration operator
    bool operator==(SpinCartesian const &other);
    // output operator
    friend std::ostream &operator<<(std::ostream &os,
                                    SpinCartesian const &s);

    SpinCartesian from_xyz(flt x, flt y, flt z);
    SpinCartesian from_ϕθ(flt ϕ, flt θ);
};


/*
x = sin(θ) ⋅ cos(ϕ)
y = sin(θ) ⋅ sin(ϕ)
z = sin(θ)

θ_byte = 0xff  =>  zero Spin

compute: inefficient, 20.77 ns, 50x slower than SpinCartesian
memory: efficient, 16Bit, 1/6 of SpinCartesian
*/
class SpinPolar
{
private:
    U8 ϕ_byte = 0;
    U8 θ_byte = 0;

public:
    flt ϕ() const;
    flt θ() const;
    flt x() const;
    flt y() const;
    flt z() const;
    // generate random spin
    SpinPolar();
    // to initialize a zero Spin
    SpinPolar(flt zero);

    SpinPolar &operator=(SpinPolar const &other);

    // scalar product (1,θ₁,ϕ₁) ⋅ (1,θ₂,ϕ₂)
    // - returns 0 if any θ=0xff
    flt operator|(SpinPolar const &other);
    // output operator
    friend std::ostream &operator<<(std::ostream &os,
                                    SpinPolar const &s);
    // compare operator
    bool operator==(SpinPolar const &other);
    // generates a spin from x,y,z
    SpinPolar from_xyz(flt x, flt y, flt z);
    // generates a spin from ϕ,θ
    SpinPolar from_ϕθ(flt ϕ, flt θ);
};

#endif // __SPIN_H__