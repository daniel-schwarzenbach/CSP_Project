#ifndef __SPIN_H__
#define __SPIN_H__
/*
    implements Spin for heissenbergvector
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
    // get a new spin from vector
    Spin from_xyz(f32 const& x, f32 const& y, f32 const& z);
    // get a new spin from polar coordinates
    Spin from_phi_theata(flt const& phi, flt const& theta);
    static Spin get_random();

    //      --- Trial moves for metropolis Step

    // flips the spin
    void spin_flip_step();
    // randomizes the spin
    void random_step();
    // does a small angle rotation
    void small_step(flt  const& openingAngle);
    // does a gaussian step for a given σ: step expectation
    void adaptive_step(flt const& sigma);
};

// templated random function: important for Lattice initialization
template <>
Spin rng::get_random<Spin>()
{
    return Spin::get_random();
}

#endif // __SPIN_H__