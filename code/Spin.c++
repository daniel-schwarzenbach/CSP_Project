#include "Spin.h++"

flt SpinCartesian::x() const { return x_; }
flt SpinCartesian::y() const { return y_; }
flt SpinCartesian::z() const { return z_; }
flt SpinCartesian::θ() const { return acos(z_); }
flt SpinCartesian::ϕ() const { return atan2(y_, x_); }

SpinCartesian::SpinCartesian()
{
    flt φ = randflt() * ₂π;
    flt ϑ = randflt() * π - π_2;
    x_ = sin(φ) * sin(ϑ);
    y_ = cos(φ) * sin(ϑ);
    z_ = cos(ϑ);
}

SpinCartesian::SpinCartesian(flt zero) : x_(0), y_(0), z_(0) {}

SpinCartesian& SpinCartesian::operator=(SpinCartesian const &other)
{
    this->x_ = other.x_;
    this->y_ = other.y_;
    this->z_ = other.z_;
    return *this;
};
// scalar product
flt SpinCartesian::operator|(SpinCartesian const &other)
{
    return this->x_ * other.x_ + this->y_ * other.y_ + this->z_ * other.z_;
}
// compareration operator
bool SpinCartesian::operator==(SpinCartesian const &other)
{
    return (abs(this->x_ == other.x_) < 1e-3) &&
            (abs(this->y_ == other.y_) < 1e-3) &&
            (abs(this->z_ == other.z_) < 1e-3);
}
// output operator
std::ostream& operator<<(std::ostream &os,
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
SpinCartesian SpinCartesian::from_ϕθ(flt ϕ, flt θ)
{
    SpinCartesian s;
    s.x_ = sin(θ) * cos(ϕ);
    s.y_ = sin(θ) * sin(ϕ);
    s.z_ = cos(θ);
    return s;
}

// @constants for SpinPolar:
constexpr flt spinpolθFac = π / 0xfe;
constexpr flt spinpolϕFac = ₂π / 0xff;
/*
x = sin(θ) ⋅ cos(ϕ)
y = sin(θ) ⋅ sin(ϕ)
z = sin(θ)

θ_byte = 0xff  =>  zero Spin

compute: inefficient, 20.77 ns, 50x slower than SpinCartesian
memory: efficient, 16Bit, 1/6 of SpinCartesian
*/

flt SpinPolar::ϕ() const { return ϕ_byte * spinpolϕFac; }
flt SpinPolar::θ() const { return θ_byte * spinpolθFac; }
flt SpinPolar::x() const { return sin(θ()) * cos(ϕ()); }
flt SpinPolar::y() const { return sin(θ()) * sin(ϕ()); }
flt SpinPolar::z() const { return cos(θ()); }
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

SpinPolar& SpinPolar::operator=(SpinPolar const &other)
{
    this->ϕ_byte = other.ϕ_byte;
    this->θ_byte = other.θ_byte;
    return *this;
};

// scalar product (1,θ₁,ϕ₁) ⋅ (1,θ₂,ϕ₂)
// - returns 0 if any θ=0xff
flt SpinPolar::operator|(SpinPolar const &other)
{
    flt θ₁ = this->θ();
    flt θ₂ = other.θ();
    if (θ₁ == 0xff || θ₂ == 0xff)
        return 0; // => zero Spin
    // Wolfram Alpha told me that
    return sin(θ₁) * sin(θ₂) * cos(this->ϕ() - other.ϕ()) + cos(θ₁) * cos(θ₂);
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
SpinPolar SpinPolar::from_ϕθ(flt ϕ, flt θ)
{
    SpinPolar s;
    s.θ_byte = round(θ / spinpolθFac);
    s.ϕ_byte = round(ϕ / spinpolϕFac);
    return s;
}
