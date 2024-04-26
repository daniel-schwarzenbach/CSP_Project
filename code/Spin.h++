#ifndef __SPIN_H__
#define __SPIN_H__

#include <Base.h++>
#include <Eigen.h++>

/*
[x, y, z]†
compute: efficient, 0.42 ns
memory: inefficient, sizeof(flt)*3 ~ (float)96bit
*/
class SpinVector : public Vector3
{
private:
    using base = Vector3;

public:
    flt x() const;
    flt y() const;
    flt z() const;
    flt theta() const;
    flt phi() const;
    // Constructor
    SpinVector(flt x = 0, flt y = 0, flt z = 0);
    // Constructor from Vector3
    SpinVector(Vector3 const &v);
    // Assignment operator
    SpinVector &operator=(SpinVector const &other);
    // Assignment operator Vector3
    SpinVector &operator=(Vector3 const &other);
    // Cast operator
    operator Vector3() const;

    // output operator
    friend std::ostream &operator<<(std::ostream &os,
                                    SpinVector const &s);
    SpinVector from_xyz(flt x, flt y, flt z);
    SpinVector from_phi_theata(flt phi, flt theta);
    static SpinVector get_random();

    // Trial moves
    void spin_flip();
    void random_move();
    void small_step_move(flt openingAngle);
    void adaptive_step(flt sigma);
};

template<>
SpinVector rng::get_random<SpinVector>(){
    return SpinVector::get_random();
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
    U8 phi_byte = 0;
    U8 theta_byte = 0;

public:
    flt phi() const;
    flt theta() const;
    flt x() const;
    flt y() const;
    flt z() const;
    // generate random spin
    SpinCompressed();
    // to initialize a zero Spin
    SpinCompressed(flt zero);

    SpinCompressed &operator=(SpinCompressed const &other);

    // scalar product (1,θ₁,ϕ₁) ⋅ (1,θ₂,ϕ₂)
    // - returns 0 if any θ=0xff
    flt operator|(SpinCompressed const &other) const;
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
    static SpinCompressed from_xyz(flt x, flt y, flt z);
    // generates a spin from ϕ,θ
    static SpinCompressed from_phi_theata(flt ϕ, flt θ);
    // generates a random spin
    static SpinCompressed get_random();
};

template<>
SpinCompressed rng::get_random<SpinCompressed>(){
    return SpinCompressed::get_random();
}

/*
[x, y, z]†
compute: efficient, 0.42 ns
memory: inefficient, sizeof(flt)*3 ~ (float)96bit
*/
// class SpinCartesian
// {
// private:
//     flt x_ = 1, y_ = 0, z_ = 0;

// public:
//     flt x() const;
//     flt y() const;
//     flt z() const;
//     flt theta() const;
//     flt phi() const;
//     // generate random spin
//     SpinCartesian();
//     // to initialize a with north pole [zero,0,0]
//     SpinCartesian(flt zero);
//     // to initialize a with x,y,z
//     SpinCartesian(flt x, flt y, flt z);
//     // assignment operator
//     SpinCartesian &operator=(SpinCartesian const &other);
//     // scalar product
//     flt operator|(SpinCartesian const &other) const;
//     // standard product
//     SpinCartesian operator*(flt const &other) const;
//     SpinCartesian &operator*=(flt const &other);
//     friend SpinCartesian operator*(flt const &lhs,
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

//     static SpinCartesian from_xyz(flt x, flt y, flt z);
//     static SpinCartesian from_phi_theata(flt ϕ, flt θ);
//     static SpinCartesian get_random();

//     // Trial moves
//     void spin_flip();
//     void random_move();
//     void small_step_move(flt openingAngle);
//     void adaptive_step(flt sigma);
//     Vector3 to_vector3() const;
// };

#endif // __SPIN_H__