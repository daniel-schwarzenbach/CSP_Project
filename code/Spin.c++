#include "Spin.h++"

flt SpinCartesian::x() const { return x_; }
flt SpinCartesian::y() const { return y_; }
flt SpinCartesian::z() const { return z_; }
flt SpinCartesian::theta() const {
    SpinCartesian s = this->normalized(); 
    return acos(s.z_); 
}
flt SpinCartesian::phi() const { 
    SpinCartesian s = this->normalized();
    return atan2(y_, x_); 
}

SpinCartesian::SpinCartesian()
{
    flt φ = randflt() * _2pi_;
    flt ϑ = randflt() * _pi_ - _pi2_;
    x_ = sin(φ) * sin(ϑ);
    y_ = cos(φ) * sin(ϑ);
    z_ = cos(ϑ);
}


SpinCartesian::SpinCartesian(flt zero) : x_(0), y_(0), z_(0) {}

SpinCartesian &SpinCartesian::operator=(SpinCartesian const &other)
{
    this->x_ = other.x_;
    this->y_ = other.y_;
    this->z_ = other.z_;
    return *this;
};
// scalar product
flt SpinCartesian::operator|(SpinCartesian const &other) const
{
    return this->x_ * other.x_ + this->y_ * other.y_ + this->z_ * other.z_;
}
// standard product
SpinCartesian SpinCartesian::operator*(SpinCartesian const &other) const
{
    SpinCartesian s;
    s.x_ = this->x_ * other.x_;
    s.y_ = this->y_ * other.y_;
    s.z_ = this->z_ * other.z_;
    return s;
}
SpinCartesian &SpinCartesian::operator*=(SpinCartesian const &other)
{
    this->x_ *= other.x_;
    this->y_ *= other.y_;
    this->z_ *= other.z_;
    return *this;
}
SpinCartesian operator*(flt const &lhs, SpinCartesian const &rhs)
{
    SpinCartesian s;
    s.x_ = lhs * rhs.x_;
    s.y_ = lhs * rhs.y_;
    s.z_ = lhs * rhs.z_;
    return s;
}
SpinCartesian SpinCartesian::operator*(flt const &other) const
{
    SpinCartesian s;
    s.x_ = this->x_ * other;
    s.y_ = this->y_ * other;
    s.z_ = this->z_ * other;
    return s;
}
SpinCartesian &SpinCartesian::operator*=(flt const &other)
{
    this->x_ *= other;
    this->y_ *= other;
    this->z_ *= other;
    return *this;
}
// addition operator
SpinCartesian SpinCartesian::operator+(SpinCartesian const &other)
    const
{
    SpinCartesian s;
    s.x_ = this->x_ + other.x_;
    s.y_ = this->y_ + other.y_;
    s.z_ = this->z_ + other.z_;
    return s;
}
SpinCartesian &SpinCartesian::operator+=(SpinCartesian const &other)
{
    this->x_ += other.x_;
    this->y_ += other.y_;
    this->z_ += other.z_;
    return *this;
}
// subtraction operator
SpinCartesian SpinCartesian::operator-(SpinCartesian const &other)
    const
{
    SpinCartesian s;
    s.x_ = this->x_ - other.x_;
    s.y_ = this->y_ - other.y_;
    s.z_ = this->z_ - other.z_;
    return s;
}
SpinCartesian &SpinCartesian::operator-=(SpinCartesian const &other)
{
    this->x_ -= other.x_;
    this->y_ -= other.y_;
    this->z_ -= other.z_;
    return *this;
}
// normalize the spin
void SpinCartesian::normalize()
{
    flt norm = sqrt(x_ * x_ + y_ * y_ + z_ * z_);
    x_ /= norm;
    y_ /= norm;
    z_ /= norm;
}
SpinCartesian SpinCartesian::normalized() const
{
    SpinCartesian s = *this;
    s.normalize();
    return s;
}
// compareration operator
bool SpinCartesian::operator==(SpinCartesian const &other) const
{
    return (abs(this->x_ - other.x_) < 1e-3) &&
           (abs(this->y_ - other.y_) < 1e-3) &&
           (abs(this->z_ - other.z_) < 1e-3);
}
// output operator
std::ostream &operator<<(std::ostream &os,
                         SpinCartesian const &s)
{
    os << "{" << s.x_ << ", " << s.y_ << ", " << s.z_ << "}";
    return os;
}

SpinCartesian SpinCartesian::from_xyz(flt x, flt y, flt z)
{
    SpinCartesian s;
    s.x_ = x;
    s.y_ = y;
    s.z_ = z;
    return s;
}
SpinCartesian SpinCartesian::from_phi_theata(flt ϕ, flt θ)
{
    SpinCartesian s;
    s.x_ = sin(θ) * cos(ϕ);
    s.y_ = sin(θ) * sin(ϕ);
    s.z_ = cos(θ);
    return s;
}

SpinCartesian SpinCartesian::get_random()
{
    SpinCartesian s;
    flt φ = randflt() * _2pi_;
    flt ϑ = randflt() * _pi_ - _pi2_;
    s.x_ = sin(φ) * sin(ϑ);
    s.y_ = cos(φ) * sin(ϑ);
    s.z_ = cos(ϑ);
    return s;
}

// Trial moves //

// Reflects the initial spin across the origin
void SpinCartesian::spin_flip() {
    x_ = -x_;
    y_ = -y_;
    z_ = -z_;
    normalize();
}

// Changes the spin to a random spin
void SpinCartesian::random_move() {
    *this = get_random();
}

// Changes the spin to a random spin within a certain distance, 
// specified by the opening angle of a cone

// Used both in the standard Metropolis when using the small step
// trial move and the adaptive Metropolis
void SpinCartesian::small_step_move(flt openingAngle) {
    // Sample angles uniformly within the cone specified by the opening angle
    std::random_device rd;
    std::mt19937 gen(rd());
    // Sampling from this interval makes sure we stay within a certain
    // distance/opening angle from the initial spin
    std::uniform_real_distribution<flt> dis_theta(0.0, openingAngle);
    std::uniform_real_distribution<flt> dis_phi(0.0, 2.0 * M_PI);

    flt theta = dis_theta(gen); // Sample theta from [0, openingAngle]
    flt phi = dis_phi(gen);     // Sample phi from [0, 2*pi)


    // Calculate the new spin vector components from the sampled angles
    // The two angles fully characterize the point on the unit sphere
    flt newX = std::sin(theta) * std::cos(phi);
    flt newY = std::sin(theta) * std::sin(phi);
    flt newZ = std::cos(theta);

    // Compute rotation quaternion from north pole (zero angles) to
    // the initial spin direction (this)
    // Needed to rotate the generated spin
    Eigen::Vector3d northPole(0.0, 0.0, 1.0); // North pole (z-axis)
    Eigen::Vector3d originalSpin(x_, y_, z_);
    Eigen::Quaterniond rotationToOriginal = Eigen::Quaterniond::FromTwoVectors(northPole, originalSpin);

    // Apply the rotation quaternion to the new spin vector
    Eigen::Vector3d rotatedNewSpin = rotationToOriginal * Eigen::Vector3d(newX, newY, newZ);

    // Update the current SpinCartesian object with the rotated spin vector components
    x_ = rotatedNewSpin.x();
    y_ = rotatedNewSpin.y();
    z_ = rotatedNewSpin.z();
}

// Performs the step of the adaptive Metropolis algorithm: adds the
// initial spin with a Gaussian distributed random vector multiplied 
// by an adaptive factor sigma
void SpinCartesian::adaptive_step(flt sigma) {
    // Initialize random number generator for Gaussian distribution
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<flt> dist(0.0, 1.0); // Mean 0.0, Standard deviation 1.0

    // Generate Gaussian-distributed random components
    flt dx = dist(gen);
    flt dy = dist(gen);
    flt dz = dist(gen);

    // Add Gaussian-distributed random vector to the spin
    x_ += sigma * dx;
    y_ += sigma * dy;
    z_ += sigma * dz;

    // Normalize the resulting spin to ensure it remains on the unit sphere
    normalize();
}

Vector3 SpinCartesian::to_vector3() const {
    return Vector3(x_, y_, z_);
}

//            --- SpinEigen ---
flt SpinEigen::x() const { return this->base::operator()(0); }
flt SpinEigen::y() const { return this->base::operator()(1); }
flt SpinEigen::z() const { return this->base::operator()(2); }

flt SpinEigen::theta() const { 
    SpinEigen s = this->base::normalized();
    return acos(s.z()); 
}
flt SpinEigen::phi() const { 
    SpinEigen s = this->base::normalized();
    return atan2(s.y(), s.x()); 
}




//            --- SpinPolar ---


// @constants for SpinPolar:
constexpr flt spinpolθFac = _pi_ / 0xfe;
constexpr flt spinpolϕFac = _2pi_ / 0xff;
/*
x = sin(θ) ⋅ cos(ϕ)
y = sin(θ) ⋅ sin(ϕ)
z = sin(θ)

θ_byte = 0xff  =>  zero Spin

compute: inefficient, 20.77 ns, 50x slower than SpinCartesian
memory: efficient, 16Bit, 1/6 of SpinCartesian
*/

flt SpinPolar::phi() const { return ϕ_byte * spinpolϕFac; }
flt SpinPolar::theta() const { return θ_byte * spinpolθFac; }
flt SpinPolar::x() const { return sin(theta()) * cos(phi()); }
flt SpinPolar::y() const { return sin(theta()) * sin(phi()); }
flt SpinPolar::z() const { return cos(theta()); }
// generate random spin
SpinPolar::SpinPolar()
{
    ϕ_byte = round(randflt() * 0xff);
    θ_byte = round(randflt() * 0xfe);
}
// to initialize a zero Spin
SpinPolar::SpinPolar(flt zero)
{
    if (zero == 0)
    {
        ϕ_byte = 0;
        θ_byte = 0xff;
    }
    else
    {
        ϕ_byte = 0;
        θ_byte = 0;
    }
}

SpinPolar &SpinPolar::operator=(SpinPolar const &other)
{
    this->ϕ_byte = other.ϕ_byte;
    this->θ_byte = other.θ_byte;
    return *this;
};

// scalar product (1,θ₁,ϕ₁) ⋅ (1,θ₂,ϕ₂)
// - returns 0 if any θ=0xff
flt SpinPolar::operator|(SpinPolar const &other) const
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
                         SpinPolar const &s)
{
    os << "{" << s.x() << ", " << s.y() << ", " << s.z() << "}";
    return os;
}
// compare operator
bool SpinPolar::operator==(SpinPolar const &other)
{
    return this->ϕ_byte == other.ϕ_byte &&
           this->θ_byte == other.θ_byte;
}

SpinPolar SpinPolar::from_xyz(flt x, flt y, flt z)
{
    SpinPolar s;
    s.θ_byte = round(acos(z) / spinpolθFac);
    s.ϕ_byte = round(atan2(y, x) / spinpolϕFac);
    return s;
}
SpinPolar SpinPolar::from_phi_theata(flt ϕ, flt θ)
{
    SpinPolar s;
    s.θ_byte = round(θ / spinpolθFac);
    s.ϕ_byte = round(ϕ / spinpolϕFac);
    return s;
}

SpinPolar SpinPolar::get_random()
{
    SpinPolar s;
    s.ϕ_byte = round(randflt() * 0xff);
    s.θ_byte = round(randflt() * 0xfe);
    return s;
}

//            --- SpinEigen ---

// flt SpinEigen::x() const { return this->operator()(0); }
// flt SpinEigen::y() const { return this->operator()(1); }
// flt SpinEigen::z() const { return this->operator()(2); }
// flt SpinEigen::theta() const { 
//     SpinEigen s = this->normalized();
//     return acos(s.z()); 
// }
// flt SpinEigen::phi() const { 
//     SpinEigen s = this->normalized();
//     return atan2(s.y(), s.x()); 
// }

// void SpinEigen::normalize()
// {
//     *this = this->normalized();
// }


// SpinEigen::SpinEigen() : Vector3() {}
// SpinEigen::SpinEigen(flt zero) : Vector3(zero, 0, 0) {}
// flt SpinEigen::operator|(SpinEigen const &other) const
// {
//     return this->dot(other);
// }

// bool SpinEigen::operator==(SpinEigen const &other) const
// {
//     return (abs(this->x() == other.x()) < 1e-3) &&
//            (abs(this->y() == other.y()) < 1e-3) &&
//            (abs(this->z() == other.z()) < 1e-3);
// }

// std::ostream &operator<<(std::ostream &os,
//                          SpinEigen const &s)
// {
//     os << "{" << s.x() << ", " << s.y() << ", " << s.z() << "}";
//     return os;
// }

// SpinEigen SpinEigen::from_xyz(flt x, flt y, flt z)
// {
//     SpinEigen s;
//     s << x, y, z;
//     return s;
// }
// SpinEigen SpinEigen::from_phi_theata(flt ϕ, flt θ)
// {
//     SpinEigen s;
//     s <<    std::sin(θ) * std::cos(ϕ), 
//             std::sin(θ) * std::sin(ϕ), 
//             std::cos(θ);
//     return s;
// }

