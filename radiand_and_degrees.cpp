#include <cmath>
#include <compare>
#include <iostream>
#include <numbers>

constexpr double pi = 3.14159265358979323846;

class radians;

class degrees {
public:
    explicit degrees(double v) : value_(v) {}

    explicit operator double() const { return value_; }

    explicit operator radians() const;

    degrees operator+(degrees rhs) const { return degrees(value_ + rhs.value_); }
    degrees operator-(degrees rhs) const { return degrees(value_ - rhs.value_); }
    degrees operator*(double k) const    { return degrees(value_ * k); }
    degrees operator/(double k) const    { return degrees(value_ / k); }

    degrees& operator+=(degrees rhs) { value_ += rhs.value_; return *this; }
    degrees& operator-=(degrees rhs) { value_ -= rhs.value_; return *this; }
    degrees& operator*=(double k)    { value_ *= k; return *this; }
    degrees& operator/=(double k)    { value_ /= k; return *this; }

    auto operator<=>(degrees const&) const = default;

    double value() const { return value_; }

private:
    double value_;
};

class radians {
public:
    explicit radians(double v) : value_(v) {}

    explicit operator double() const { return value_; }

    explicit operator degrees() const {
        return degrees(value_ * 180.0 / pi);
    }

    radians operator+(radians rhs) const { return radians(value_ + rhs.value_); }
    radians operator-(radians rhs) const { return radians(value_ - rhs.value_); }
    radians operator*(double k) const    { return radians(value_ * k); }
    radians operator/(double k) const    { return radians(value_ / k); }

    radians& operator+=(radians rhs) { value_ += rhs.value_; return *this; }
    radians& operator-=(radians rhs) { value_ -= rhs.value_; return *this; }
    radians& operator*=(double k)    { value_ *= k; return *this; }
    radians& operator/=(double k)    { value_ /= k; return *this; }

    auto operator<=>(radians const&) const = default;

    double value() const { return value_; }

private:
    double value_;
};

inline degrees::operator radians() const {
    return radians(value_ * pi / 180.0);
}
inline degrees operator"" _deg(unsigned long long v) {
    return degrees(static_cast<double>(v));
}
inline degrees operator"" _deg(long double v) {
    return degrees(static_cast<double>(v));
}

inline radians operator"" _rad(unsigned long long v) {
    return radians(static_cast<double>(v));
}
inline radians operator"" _rad(long double v) {
    return radians(static_cast<double>(v));
}

int main() {
    degrees a = 90_deg;
    radians b = 1.57079632679_rad;

    radians a_rad = (radians)a;
    degrees b_deg = (degrees)b;

    std::cout << "a in radians = " << (double)a_rad << "\n";
    std::cout << "b in degrees = " << (double)b_deg << "\n";

    degrees c = a + 15_deg;
    radians d = b * 2.0;

    std::cout << "c = " << (double)c << " deg\n";
    std::cout << "d = " << (double)d << " rad\n";
}
