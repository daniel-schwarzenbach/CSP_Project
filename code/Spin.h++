#ifndef __SPIN_H__
#define __SPIN_H__

#include <Base.h++>
#include <Eigen.h++>

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
    flt theta() const;
    flt phi() const;
    // generate random spin
    SpinCartesian();
    SpinCartesian(flt zero);
    SpinCartesian &operator=(SpinCartesian const &other);
    // scalar product
    flt operator|(SpinCartesian const &other) const;
    // standard product
    SpinCartesian operator*(flt const &other) const;
    SpinCartesian &operator*=(flt const &other);
    friend SpinCartesian operator*(flt const &lhs,
                                   SpinCartesian const &rhs);
    SpinCartesian operator*(SpinCartesian const &other) const;
    SpinCartesian &operator*=(SpinCartesian const &other);
    // addition operator
    SpinCartesian operator+(SpinCartesian const &other) const;
    SpinCartesian &operator+=(SpinCartesian const &other);
    // subtraction operator
    SpinCartesian operator-(SpinCartesian const &other) const;
    SpinCartesian &operator-=(SpinCartesian const &other);
    // return a normalized copy of the spin
    SpinCartesian normalized() const;
    // normalize the spin
    void normalize();
    // compareration operator
    bool operator==(SpinCartesian const &other) const;
    // output operator
    friend std::ostream &operator<<(std::ostream &os,
                                    SpinCartesian const &s);

    SpinCartesian from_xyz(flt x, flt y, flt z);
    SpinCartesian from_phi_theata(flt ϕ, flt θ);
    SpinCartesian get_random();
};

/*
[x, y, z]†
compute: efficient, 0.42 ns
memory: inefficient, sizeof(flt)*3 ~ (float)96bit
*/
class SpinEigen : public Vector3
{
private:
    using base = Vector3;
public:
    using base::base;
    using base::operator=;
    flt x() const;
    flt y() const;
    flt z() const;
    flt theta() const;
    flt phi() const;
    SpinEigen();
    SpinEigen(flt zero);
    SpinEigen &operator=(SpinEigen const &other);
    // scalar product
    flt operator|(SpinEigen const &other) const;
    // normalize the spin
    void normalize();
    // compareration operator
    bool operator==(SpinEigen const &other) const;
    // output operator
    friend std::ostream &operator<<(std::ostream &os,
                                    SpinEigen const &s);
    SpinEigen from_xyz(flt x, flt y, flt z);
    SpinEigen from_phi_theata(flt ϕ, flt θ);
    SpinEigen get_random();
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
    flt phi() const;
    flt theta() const;
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
    flt operator|(SpinPolar const &other) const;
    // standard product: is always normalized!
    SpinPolar operator*(SpinPolar const &other) const;
    SpinPolar &operator*=(SpinPolar const &other);
    // addition operator: is always normalized!
    SpinPolar operator+(SpinPolar const &other) const;
    SpinPolar &operator+=(SpinPolar const &other);
    // subtraction operator: is always normalized!
    SpinPolar operator-(SpinPolar const &other) const;
    SpinPolar &operator-=(SpinPolar const &other);
    // does nothing
    void normalize(){};
    // output operator
    friend std::ostream &operator<<(std::ostream &os,
                                    SpinPolar const &s);
    // compare operator
    bool operator==(SpinPolar const &other);
    // generates a spin from x,y,z
    SpinPolar from_xyz(flt x, flt y, flt z);
    // generates a spin from ϕ,θ
    SpinPolar from_phi_theata(flt ϕ, flt θ);
    // generates a random spin
    SpinPolar get_random();
};

#endif // __SPIN_H__