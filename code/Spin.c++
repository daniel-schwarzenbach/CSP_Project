#include "Spin.h++"

//            --- SpinVector ---
SpinVector::SpinVector(Vector3 const &v) : base(v) {}
SpinVector &SpinVector::operator=(Vector3 const &other)
{
    this->base::operator=(other);
    return *this;
}

flt SpinVector::x() const { return this->base::operator()(0); }
flt SpinVector::y() const { return this->base::operator()(1); }
flt SpinVector::z() const { return this->base::operator()(2); }

flt SpinVector::theta() const
{
    SpinVector s = this->base::normalized();
    return acos(s.z());
}
flt SpinVector::phi() const
{
    SpinVector s = this->normalized();
    flt phi = atan2(s.y(), s.x());
    if (phi < 0)
        phi += _2pi_;
    return phi;
}

SpinVector::SpinVector(flt x, flt y, flt z) : base(x, y, z) {}
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

SpinVector SpinVector::from_xyz(flt x, flt y, flt z)
{
    SpinVector s;
    s << x, y, z;
    return s;
}
SpinVector SpinVector::from_phi_theata(flt phi, flt theta)
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
    F64 theta = rng::rand_uniform() * _pi_;
    F64 phi = rng::rand_uniform() * _2pi_;
    s << std::sin(theta) * std::cos(phi),
        std::sin(theta) * std::sin(phi),
        std::cos(theta);
    return s;
}

void SpinVector::spin_flip()
{
    *this *= -1;
}
void SpinVector::random_move()
{
    *this = SpinVector::get_random();
}
void SpinVector::small_step_move(F64 openingAngle)
{
    F64 theta = rng::rand_gaussion() * openingAngle;
    F64 phi = rng::rand_gaussion() * _2pi_;
    Vector3 randomPole;
    randomPole << std::sin(theta) * std::cos(phi),
        std::sin(theta) * std::sin(phi),
        std::cos(theta);
    Vector3 northPole(0, 0, 1);
    Eigen::Quaternion<flt> rotationToOriginal =
        Eigen::Quaternion<flt>::FromTwoVectors(
            northPole, *this);
    *this = rotationToOriginal * randomPole;
}

void SpinVector::adaptive_step(F64 sigma)
{
    F64 dx = rng::rand_gaussion();
    F64 dy = rng::rand_gaussion();
    F64 dz = rng::rand_gaussion();
    *this += sigma * SpinVector(dx, dy, dz);
    this->normalize();
}

//            --- SpinCompressed ---

// @constants for SpinCompressed:
constexpr flt thetaFac = _pi_ / 0xfe;
constexpr flt phiFac = _2pi_ / 0xff;
/*
x = sin(theta) ⋅ cos(phi)
y = sin(theta) ⋅ sin(phi)
z = sin(theta)

theta_byte = 0xff  =>  zero Spin

compute: inefficient, 20.77 ns, 50x slower than SpinCartesian
memory: efficient, 16Bit, 1/6 of SpinCartesian
*/

flt SpinCompressed::phi() const { return phi_byte * phiFac; }
flt SpinCompressed::theta() const { return theta_byte * thetaFac; }
flt SpinCompressed::x() const { return sin(theta()) * cos(phi()); }
flt SpinCompressed::y() const { return sin(theta()) * sin(phi()); }
flt SpinCompressed::z() const { return cos(theta()); }

// generate random spin
SpinCompressed::SpinCompressed()
{
    phi_byte = round(rng::rand_uniform() * 0xff);
    theta_byte = round(rng::rand_uniform() * 0xfe);
}
// to initialize a zero Spin
SpinCompressed::SpinCompressed(flt zero)
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
flt SpinCompressed::operator|(SpinCompressed const &other) const
{
    flt t1 = this->theta();
    flt t2 = other.theta();
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

SpinCompressed SpinCompressed::from_xyz(flt x, flt y, flt z)
{
    SpinCompressed s;
    s.theta_byte = round(acos(z) / thetaFac);
    s.phi_byte = round(atan2(y, x) / phiFac);
    return s;
}
SpinCompressed SpinCompressed::from_phi_theata(flt phi, flt theta)
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

// flt SpinCartesian::x() const { return x_; }
// flt SpinCartesian::y() const { return y_; }
// flt SpinCartesian::z() const { return z_; }
// flt SpinCartesian::theta() const
// {
//     SpinCartesian s = this->normalized();
//     return acos(s.z_);
// }
// flt SpinCartesian::phi() const
// {
//     SpinCartesian s = this->normalized();
//     return atan2(y_, x_);
// }

// SpinCartesian::SpinCartesian(flt x, flt y, flt z)
//     : x_(x), y_(y), z_(z) {}

// SpinCartesian &SpinCartesian::operator=(SpinCartesian const &other)
// {
//     this->x_ = other.x_;
//     this->y_ = other.y_;
//     this->z_ = other.z_;
//     return *this;
// };
// // scalar product
// flt SpinCartesian::operator|(SpinCartesian const &other) const
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
// SpinCartesian operator*(flt const &lhs, SpinCartesian const &rhs)
// {
//     SpinCartesian s;
//     s.x_ = lhs * rhs.x_;
//     s.y_ = lhs * rhs.y_;
//     s.z_ = lhs * rhs.z_;
//     return s;
// }
// SpinCartesian SpinCartesian::operator*(flt const &other) const
// {
//     SpinCartesian s;
//     s.x_ = this->x_ * other;
//     s.y_ = this->y_ * other;
//     s.z_ = this->z_ * other;
//     return s;
// }
// SpinCartesian &SpinCartesian::operator*=(flt const &other)
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
//     flt norm = sqrt(x_ * x_ + y_ * y_ + z_ * z_);
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

// SpinCartesian SpinCartesian::from_xyz(flt x, flt y, flt z)
// {
//     SpinCartesian s;
//     s.x_ = x;
//     s.y_ = y;
//     s.z_ = z;
//     return s;
// }
// SpinCartesian SpinCartesian::from_phi_theata(flt phi, flt theta)
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
//     flt phi = rng::randflt() * _2pi_;
//     flt theta = rng::randflt() * _pi_;
//     s.x_ = sin(phi) * sin(theta);
//     s.y_ = cos(phi) * sin(theta);
//     s.z_ = cos(theta);
//     return s;
// }

// // Trial moves //

// // Reflects the initial spin across the origin
// void SpinCartesian::spin_flip()
// {
//     x_ = -x_;
//     y_ = -y_;
//     z_ = -z_;
//     normalize();
// }

// // Changes the spin to a random spin
// void SpinCartesian::random_move()
// {
//     *this = get_random();
// }

// // Changes the spin to a random spin within a certain distance,
// // specified by the opening angle of a cone

// // Used both in the standard Metropolis when using the small step
// // trial move and the adaptive Metropolis
// void SpinCartesian::small_step_move(flt openingAngle)
// {
//     // Sample angles uniformly within the cone specified by the opening angle
//     std::random_device rd;
//     std::mt19937 gen(rd());
//     // Sampling from this interval makes sure we stay within a certain
//     // distance/opening angle from the initial spin
//     std::uniform_real_distribution<flt> dis_theta(0.0, openingAngle);
//     std::uniform_real_distribution<flt> dis_phi(0.0, 2.0 * M_PI);

//     flt theta = dis_theta(gen); // Sample theta from [0, openingAngle]
//     flt phi = dis_phi(gen);     // Sample phi from [0, 2*pi)

//     // Calculate the new spin vector components from the sampled angles
//     // The two angles fully characterize the point on the unit sphere
//     flt newX = std::sin(theta) * std::cos(phi);
//     flt newY = std::sin(theta) * std::sin(phi);
//     flt newZ = std::cos(theta);

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
// void SpinCartesian::adaptive_step(flt sigma)
// {
//     // Initialize random number generator for Gaussian distribution
//     std::random_device rd;
//     std::mt19937 gen(rd());
//     std::normal_distribution<flt> dist(0.0, 1.0); // Mean 0.0, Standard deviation 1.0

//     // Generate Gaussian-distributed random components
//     flt dx = dist(gen);
//     flt dy = dist(gen);
//     flt dz = dist(gen);

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
