#ifndef __SPIN_H__
#define __SPIN_H__
/*
    implements Heisenberg Spin
*/

#include <Base.h++>
#include <Eigen.h++>

/*
Spin class
- Vector3 aka Eigen::Vector3f
- with scalar product: |
- with standard product: *
- with addition operator: +
- with subtraction operator: -
- with normalization: normalize()
- with output operator: <<
- with compareration operator: ==
- with assignment operator: =
*/
class Spin : public Vector3
{
private:
    using base = Vector3;

public:
    f32 x() const;
    f32 y() const;
    f32 z() const;
    f32 theta() const;
    f32 phi() const;
    // Constructor
    Spin(f32 x = 0, f32 y = 0, f32 z = 0);
    // Constructor from Vector3
    Spin(Vector3 const &v);
    // Assignment operator
    Spin &operator=(Spin const &other);
    // Assignment operator Vector3
    Spin &operator=(Vector3 const &other);
    // output operator
    friend std::ostream &operator<<(std::ostream &os,
                                    Spin const &s);
    Spin from_xyz(f32 x, f32 y, f32 z);
    Spin from_phi_theata(f32 phi, f32 theta);
    static Spin get_random();

    // Trial moves
    void spin_flip_step();
    void random_step();
    void small_step(flt openingAngle);
    void adaptive_step(flt sigma);
};

template <>
Spin rng::get_random<Spin>()
{
    return Spin::get_random();
}

/*
x = sin(θ) ⋅ cos(ϕ)
y = sin(θ) ⋅ sin(ϕ)
z = sin(θ)

theta_byte = 0xff  =>  zero Spin

compute: inefficient, 20.77 ns, 50x slower than SpinCartesian
memory: efficient, 16Bit, 1/6 of SpinCartesian
*/
class SpinCompressed
{
private:
    u8 phi_byte = 0;
    u8 theta_byte = 0;

public:
    f32 phi() const;
    f32 theta() const;
    f32 x() const;
    f32 y() const;
    f32 z() const;
    // generate random spin
    SpinCompressed();
    // to initialize a zero Spin
    SpinCompressed(f32 zero);

    SpinCompressed &operator=(SpinCompressed const &other);

    // scalar product (1,θ₁,ϕ₁) ⋅ (1,θ₂,ϕ₂)
    // - returns 0 if any θ=0xff
    f32 operator|(SpinCompressed const &other) const;
    // standard product: is always normalized!
    SpinCompressed operator*(SpinCompressed const &other) const;
    SpinCompressed &operator*=(SpinCompressed const &other);
    // addition operator: is always normalized!
    SpinCompressed operator+(SpinCompressed const &other) const;
    SpinCompressed &operator+=(SpinCompressed const &other);
    // subtraction operator: is always normalized!
    SpinCompressed operator-(SpinCompressed const &other) const;
    SpinCompressed &operator-=(SpinCompressed const &other);
    // does nothing
    void normalize(){};
    // output operator
    friend std::ostream &operator<<(std::ostream &os,
                                    SpinCompressed const &s);
    // compare operator
    bool operator==(SpinCompressed const &other);
    // generates a spin from x,y,z
    static SpinCompressed from_xyz(f32 x, f32 y, f32 z);
    // generates a spin from ϕ,θ
    static SpinCompressed from_phi_theata(f32 ϕ, f32 θ);
    // generates a random spin
    static SpinCompressed get_random();
};

template <>
SpinCompressed rng::get_random<SpinCompressed>()
{
    return SpinCompressed::get_random();
}

/*
[x, y, z]†
compute: efficient, 0.42 ns
memory: inefficient, sizeof(f32)*3 ~ (float)96bit
*/
// class SpinCartesian
// {
// private:
//     f32 x_ = 1, y_ = 0, z_ = 0;

// public:
//     f32 x() const;
//     f32 y() const;
//     f32 z() const;
//     f32 theta() const;
//     f32 phi() const;
//     // generate random spin
//     SpinCartesian();
//     // to initialize a with north pole [zero,0,0]
//     SpinCartesian(f32 zero);
//     // to initialize a with x,y,z
//     SpinCartesian(f32 x, f32 y, f32 z);
//     // assignment operator
//     SpinCartesian &operator=(SpinCartesian const &other);
//     // scalar product
//     f32 operator|(SpinCartesian const &other) const;
//     // standard product
//     SpinCartesian operator*(f32 const &other) const;
//     SpinCartesian &operator*=(f32 const &other);
//     friend SpinCartesian operator*(f32 const &lhs,
//                                    SpinCartesian const &rhs);
//     SpinCartesian operator*(SpinCartesian const &other) const;
//     SpinCartesian &operator*=(SpinCartesian const &other);
//     // addition operator
//     SpinCartesian operator+(SpinCartesian const &other) const;
//     SpinCartesian &operator+=(SpinCartesian const &other);
//     // subtraction operator
//     SpinCartesian operator-(SpinCartesian const &other) const;
//     SpinCartesian &operator-=(SpinCartesian const &other);
//     // return a normalized copy of the spin
//     SpinCartesian normalized() const;
//     // normalize the spin
//     void normalize();
//     // compareration operator
//     bool operator==(SpinCartesian const &other) const;
//     // output operator
//     friend std::ostream &operator<<(std::ostream &os,
//                                     SpinCartesian const &s);

//     static SpinCartesian from_xyz(f32 x, f32 y, f32 z);
//     static SpinCartesian from_phi_theata(f32 ϕ, f32 θ);
//     static SpinCartesian get_random();

//     // Trial moves
//     void spin_flip_step();
//     void random_step();
//     void small_step(f32 openingAngle);
//     void adaptive_step(f32 sigma);
//     Vector3 to_vector3() const;
// };

#endif // __SPIN_H__