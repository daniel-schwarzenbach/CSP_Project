#include "Spin.h++"

//            --- Spin ---
Spin::Spin(Vector3 const &v) : base(v) {}
Spin &Spin::operator=(Vector3 const &other)
{
    this->base::operator=(other);
    return *this;
}

f32 Spin::x() const { return this->base::operator()(0); }
f32 Spin::y() const { return this->base::operator()(1); }
f32 Spin::z() const { return this->base::operator()(2); }

f32 Spin::theta() const
{
    Spin s = this->base::normalized();
    return acos(s.z());
}
f32 Spin::phi() const
{
    Spin s = this->normalized();
    f32 phi = atan2(s.y(), s.x());
    if (phi < 0)
        phi += _2pi_;
    return phi;
}

Spin::Spin(f32 x, f32 y, f32 z) : base(x, y, z) {}
Spin &Spin::operator=(Spin const &other)
{
    this->base::operator=(other);
    return *this;
}

std::ostream &operator<<(std::ostream &os,
                         Spin const &s)
{
    // add to the outstream: {x, y, z}
    os << "{" << s.x() << ", " << s.y() << ", " << s.z() << "}";
    return os;
}

Spin Spin::from_xyz(f32 const& x, f32 const& y, f32  const& z)
{
    Spin s;
    s << x, y, z;
    return s;
}
Spin Spin::from_phi_theata(flt const& phi, flt const& theta)
{
    Spin s;
    // build spin from polar coordinates
    s << std::sin(theta) * std::cos(phi),
        std::sin(theta) * std::sin(phi),
        std::cos(theta);
    return s;
}


Spin Spin::get_random()
{
    // generate random vector
    f32 z = rng::rand_uniform_singed();
    f32 y = rng::rand_uniform_singed();
    f32 x = rng::rand_uniform_singed();
    Spin s = {x,y,z};
    // normalize the spin
    return s.normalized();
}


// Trial moves

void Spin::spin_flip_step()
{
    // flip the spin
    *this *= -1;
}
void Spin::random_step()
{
    // set the vector to random
    *this = get_random();
}

void Spin::small_step(flt const& openingAngle)
{
    // define random angles
    flt theta = rng::rand_uniform_singed() * openingAngle;
    flt phi = rng::rand_uniform() * _2pi_;
    // set to a random Spin
    Vector3 randomPole;
    randomPole << std::sin(theta) * std::cos(phi),
        std::sin(theta) * std::sin(phi),
        std::cos(theta);
    // define the north pole
    Vector3 northPole(0, 0, 1);
    // define the rotation to the the current vector
    Eigen::Quaternion<f32> rotationToOriginal =
        Eigen::Quaternion<f32>::FromTwoVectors(
            northPole, *this);
    // rotate the randomPole with the rotation
    *this = rotationToOriginal * randomPole;
    // normalize the vector for security
    this->normalize();
}

void Spin::adaptive_step(flt const& sigma)
{
    // define a 3D gausion step
    flt dx = rng::rand_gaussian();
    flt dy = rng::rand_gaussian();
    flt dz = rng::rand_gaussian();
    // take this step with factor sigma
    *this += sigma * Spin(dx, dy, dz);
    // normalize the Spine
    this->normalize();
}
