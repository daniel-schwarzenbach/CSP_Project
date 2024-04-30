#include "Spin.h++"

//            --- SpinVector ---
SpinVector::SpinVector(Vector3 const &v) : base(v) {}
SpinVector &SpinVector::operator=(Vector3 const &other)
{
    this->base::operator=(other);
    return *this;
}

f32 SpinVector::x() const { return this->base::operator()(0); }
f32 SpinVector::y() const { return this->base::operator()(1); }
f32 SpinVector::z() const { return this->base::operator()(2); }

f32 SpinVector::theta() const
{
    SpinVector s = this->base::normalized();
    return acos(s.z());
}
f32 SpinVector::phi() const
{
    SpinVector s = this->normalized();
    f32 phi = atan2(s.y(), s.x());
    if (phi < 0)
        phi += _2pi_;
    return phi;
}

SpinVector::SpinVector(f32 x, f32 y, f32 z) : base(x, y, z) {}
SpinVector &SpinVector::operator=(SpinVector const &other)
{
    this->base::operator=(other);
    return *this;
}

SpinVector::operator Vector3() const
{
    return this->base::normalized();
}

std::ostream &operator<<(std::ostream &os,
                         SpinVector const &s)
{
    os << "{" << s.x() << ", " << s.y() << ", " << s.z() << "}";
    return os;
}

SpinVector SpinVector::from_xyz(f32 x, f32 y, f32 z)
{
    SpinVector s;
    s << x, y, z;
    return s;
}
SpinVector SpinVector::from_phi_theata(f32 phi, f32 theta)
{
    SpinVector s;
    s << std::sin(theta) * std::cos(phi),
        std::sin(theta) * std::sin(phi),
        std::cos(theta);
    return s;
}

SpinVector SpinVector::get_random()
{
    SpinVector s;
    flt theta = rng::rand_uniform() * _pi_;
    flt phi = rng::rand_uniform() * _2pi_;
    s << std::sin(theta) * std::cos(phi),
        std::sin(theta) * std::sin(phi),
        std::cos(theta);
    return s;
}

void SpinVector::spin_flip_step()
{
    *this *= -1;
}
void SpinVector::random_step()
{
    *this += get_random()*rng::rand_gaussion();
    this->normalize();
}

void SpinVector::small_step(flt openingAngle)
{
    flt theta = rng::rand_gaussion() * openingAngle;
    flt phi = rng::rand_uniform() * _2pi_;
    Vector3 randomPole;
    randomPole << std::sin(theta) * std::cos(phi),
        std::sin(theta) * std::sin(phi),
        std::cos(theta);
    Vector3 northPole(0, 0, 1);
    Eigen::Quaternion<f32> rotationToOriginal =
        Eigen::Quaternion<f32>::FromTwoVectors(
            northPole, *this);
    *this = rotationToOriginal * randomPole;
}

void SpinVector::adaptive_step(flt sigma)
{
    flt dx = rng::rand_gaussion();
    flt dy = rng::rand_gaussion();
    flt dz = rng::rand_gaussion();
    *this += sigma * SpinVector(dx, dy, dz);
    this->normalize();
}

//            --- SpinCompressed ---

// @constants for SpinCompressed:
constexpr f32 thetaFac = _pi_ / 0xfe;
constexpr f32 phiFac = _2pi_ / 0xff;
/*
x = sin(theta) ⋅ cos(phi)
y = sin(theta) ⋅ sin(phi)
z = sin(theta)

theta_byte = 0xff  =>  zero Spin

compute: inefficient, 20.77 ns, 50x slower than SpinCartesian
memory: efficient, 16Bit, 1/6 of SpinCartesian
*/

f32 SpinCompressed::phi() const { return phi_byte * phiFac; }
f32 SpinCompressed::theta() const { return theta_byte * thetaFac; }
f32 SpinCompressed::x() const { return sin(theta()) * cos(phi()); }
f32 SpinCompressed::y() const { return sin(theta()) * sin(phi()); }
f32 SpinCompressed::z() const { return cos(theta()); }

// generate random spin
SpinCompressed::SpinCompressed()
{
    phi_byte = round(rng::rand_uniform() * 0xff);
    theta_byte = round(rng::rand_uniform() * 0xfe);
}
// to initialize a zero Spin
SpinCompressed::SpinCompressed(f32 zero)
{
    if (zero == 0)
    {
        phi_byte = 0;
        theta_byte = 0xff;
    }
    else
    {
        phi_byte = 0;
        theta_byte = 0;
    }
}

SpinCompressed &SpinCompressed::operator=(SpinCompressed const &other)
{
    this->phi_byte = other.phi_byte;
    this->theta_byte = other.theta_byte;
    return *this;
};

// scalar product (1,θ₁,ϕ₁) ⋅ (1,θ₂,ϕ₂)
// - returns 0 if any theta=0xff
f32 SpinCompressed::operator|(SpinCompressed const &other) const
{
    f32 t1 = this->theta();
    f32 t2 = other.theta();
    if (t1 == 0xff || t2 == 0xff)
        return 0; // => zero Spin
    // Wolfram Alpha told me that
    return sin(t1) * sin(t2) * cos(this->phi() - other.phi()) + cos(t1) * cos(t2);
}
// output operator
std::ostream &operator<<(std::ostream &os,
                         SpinCompressed const &s)
{
    os << "{" << s.x() << ", " << s.y() << ", " << s.z() << "}";
    return os;
}
// compare operator
bool SpinCompressed::operator==(SpinCompressed const &other)
{
    return this->phi_byte == other.phi_byte &&
           this->theta_byte == other.theta_byte;
}

SpinCompressed SpinCompressed::from_xyz(f32 x, f32 y, f32 z)
{
    SpinCompressed s;
    s.theta_byte = round(acos(z) / thetaFac);
    s.phi_byte = round(atan2(y, x) / phiFac);
    return s;
}
SpinCompressed SpinCompressed::from_phi_theata(f32 phi, f32 theta)
{
    SpinCompressed s;
    s.theta_byte = round(theta / thetaFac);
    s.phi_byte = round(phi / phiFac);
    return s;
}

SpinCompressed SpinCompressed::get_random()
{
    SpinCompressed s;
    s.phi_byte = round(rng::rand_uniform() * 0xff);
    s.theta_byte = round(rng::rand_uniform() * 0xfe);
    return s;
}

// f32 SpinCartesian::x() const { return x_; }
// f32 SpinCartesian::y() const { return y_; }
// f32 SpinCartesian::z() const { return z_; }
// f32 SpinCartesian::theta() const
// {
//     SpinCartesian s = this->normalized();
//     return acos(s.z_);
// }
// f32 SpinCartesian::phi() const
// {
//     SpinCartesian s = this->normalized();
//     return atan2(y_, x_);
// }

// SpinCartesian::SpinCartesian(f32 x, f32 y, f32 z)
//     : x_(x), y_(y), z_(z) {}

// SpinCartesian &SpinCartesian::operator=(SpinCartesian const &other)
// {
//     this->x_ = other.x_;
//     this->y_ = other.y_;
//     this->z_ = other.z_;
//     return *this;
// };
// // scalar product
// f32 SpinCartesian::operator|(SpinCartesian const &other) const
// {
//     return this->x_ * other.x_ + this->y_ * other.y_ + this->z_ * other.z_;
// }
// // standard product
// SpinCartesian SpinCartesian::operator*(SpinCartesian const &other) const
// {
//     SpinCartesian s;
//     s.x_ = this->x_ * other.x_;
//     s.y_ = this->y_ * other.y_;
//     s.z_ = this->z_ * other.z_;
//     return s;
// }
// SpinCartesian &SpinCartesian::operator*=(SpinCartesian const &other)
// {
//     this->x_ *= other.x_;
//     this->y_ *= other.y_;
//     this->z_ *= other.z_;
//     return *this;
// }
// SpinCartesian operator*(f32 const &lhs, SpinCartesian const &rhs)
// {
//     SpinCartesian s;
//     s.x_ = lhs * rhs.x_;
//     s.y_ = lhs * rhs.y_;
//     s.z_ = lhs * rhs.z_;
//     return s;
// }
// SpinCartesian SpinCartesian::operator*(f32 const &other) const
// {
//     SpinCartesian s;
//     s.x_ = this->x_ * other;
//     s.y_ = this->y_ * other;
//     s.z_ = this->z_ * other;
//     return s;
// }
// SpinCartesian &SpinCartesian::operator*=(f32 const &other)
// {
//     this->x_ *= other;
//     this->y_ *= other;
//     this->z_ *= other;
//     return *this;
// }
// // addition operator
// SpinCartesian SpinCartesian::operator+(SpinCartesian const &other)
//     const
// {
//     SpinCartesian s;
//     s.x_ = this->x_ + other.x_;
//     s.y_ = this->y_ + other.y_;
//     s.z_ = this->z_ + other.z_;
//     return s;
// }
// SpinCartesian &SpinCartesian::operator+=(SpinCartesian const &other)
// {
//     this->x_ += other.x_;
//     this->y_ += other.y_;
//     this->z_ += other.z_;
//     return *this;
// }
// // subtraction operator
// SpinCartesian SpinCartesian::operator-(SpinCartesian const &other)
//     const
// {
//     SpinCartesian s;
//     s.x_ = this->x_ - other.x_;
//     s.y_ = this->y_ - other.y_;
//     s.z_ = this->z_ - other.z_;
//     return s;
// }
// SpinCartesian &SpinCartesian::operator-=(SpinCartesian const &other)
// {
//     this->x_ -= other.x_;
//     this->y_ -= other.y_;
//     this->z_ -= other.z_;
//     return *this;
// }
// // normalize the spin
// void SpinCartesian::normalize()
// {
//     f32 norm = sqrt(x_ * x_ + y_ * y_ + z_ * z_);
//     x_ /= norm;
//     y_ /= norm;
//     z_ /= norm;
// }
// SpinCartesian SpinCartesian::normalized() const
// {
//     SpinCartesian s = *this;
//     s.normalize();
//     return s;
// }
// // compareration operator
// bool SpinCartesian::operator==(SpinCartesian const &other) const
// {
//     return (abs(this->x_ - other.x_) < 1e-3) &&
//            (abs(this->y_ - other.y_) < 1e-3) &&
//            (abs(this->z_ - other.z_) < 1e-3);
// }
// // output operator
// std::ostream &operator<<(std::ostream &os,
//                          SpinCartesian const &s)
// {
//     os << "{" << s.x_ << ", " << s.y_ << ", " << s.z_ << "}";
//     return os;
// }

// SpinCartesian SpinCartesian::from_xyz(f32 x, f32 y, f32 z)
// {
//     SpinCartesian s;
//     s.x_ = x;
//     s.y_ = y;
//     s.z_ = z;
//     return s;
// }
// SpinCartesian SpinCartesian::from_phi_theata(f32 phi, f32 theta)
// {
//     SpinCartesian s;
//     s.x_ = sin(theta) * cos(phi);
//     s.y_ = sin(theta) * sin(phi);
//     s.z_ = cos(theta);
//     return s;
// }

// SpinCartesian SpinCartesian::get_random()
// {
//     SpinCartesian s;
//     f32 phi = rng::randflt() * _2pi_;
//     f32 theta = rng::randflt() * _pi_;
//     s.x_ = sin(phi) * sin(theta);
//     s.y_ = cos(phi) * sin(theta);
//     s.z_ = cos(theta);
//     return s;
// }

// // Trial moves //

// // Reflects the initial spin across the origin
// void SpinCartesian::spin_flip_step()
// {
//     x_ = -x_;
//     y_ = -y_;
//     z_ = -z_;
//     normalize();
// }

// // Changes the spin to a random spin
// void SpinCartesian::random_step()
// {
//     *this = get_random();
// }

// // Changes the spin to a random spin within a certain distance,
// // specified by the opening angle of a cone

// // Used both in the standard Metropolis when using the small step
// // trial move and the adaptive Metropolis
// void SpinCartesian::small_step(f32 openingAngle)
// {
//     // Sample angles uniformly within the cone specified by the opening angle
//     std::random_device rd;
//     std::mt19937 gen(rd());
//     // Sampling from this interval makes sure we stay within a certain
//     // distance/opening angle from the initial spin
//     std::uniform_real_distribution<f32> dis_theta(0.0, openingAngle);
//     std::uniform_real_distribution<f32> dis_phi(0.0, 2.0 * M_PI);

//     f32 theta = dis_theta(gen); // Sample theta from [0, openingAngle]
//     f32 phi = dis_phi(gen);     // Sample phi from [0, 2*pi)

//     // Calculate the new spin vector components from the sampled angles
//     // The two angles fully characterize the point on the unit sphere
//     f32 newX = std::sin(theta) * std::cos(phi);
//     f32 newY = std::sin(theta) * std::sin(phi);
//     f32 newZ = std::cos(theta);

//     // Compute rotation quaternion from north pole (zero angles) to
//     // the initial spin direction (this)
//     // Needed to rotate the generated spin
//     Eigen::Vector3d northPole(0.0, 0.0, 1.0); // North pole (z-axis)
//     Eigen::Vector3d originalSpin(x_, y_, z_);
//     Eigen::Quaterniond rotationToOriginal = Eigen::Quaterniond::FromTwoVectors(northPole, originalSpin);

//     // Apply the rotation quaternion to the new spin vector
//     Eigen::Vector3d rotatedNewSpin = rotationToOriginal * Eigen::Vector3d(newX, newY, newZ);

//     // Update the current SpinCartesian object with the rotated spin vector components
//     x_ = rotatedNewSpin.x();
//     y_ = rotatedNewSpin.y();
//     z_ = rotatedNewSpin.z();
// }

// // Performs the step of the adaptive Metropolis algorithm: adds the
// // initial spin with a Gaussian distributed random vector multiplied
// // by an adaptive factor sigma
// void SpinCartesian::adaptive_step(f32 sigma)
// {
//     // Initialize random number generator for Gaussian distribution
//     std::random_device rd;
//     std::mt19937 gen(rd());
//     std::normal_distribution<f32> dist(0.0, 1.0); // Mean 0.0, Standard deviation 1.0

//     // Generate Gaussian-distributed random components
//     f32 dx = dist(gen);
//     f32 dy = dist(gen);
//     f32 dz = dist(gen);

//     // Add Gaussian-distributed random vector to the spin
//     x_ += sigma * dx;
//     y_ += sigma * dy;
//     z_ += sigma * dz;

//     // Normalize the resulting spin to ensure it remains on the unit sphere
//     normalize();
// }

// Vector3 SpinCartesian::to_vector3() const
// {
//     return Vector3(x_, y_, z_);
// }
