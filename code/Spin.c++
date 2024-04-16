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

