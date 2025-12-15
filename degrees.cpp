#include "degrees.h"
#include "radians.h"

degrees::degrees(double v) : value_(v) {}

degrees::operator double() const {
    return value_;
}

degrees::operator radians() const {
    return radians(value_ * pi / 180.0);
}

degrees degrees::operator+(degrees rhs) const {
    return degrees(value_ + rhs.value_);
}

degrees degrees::operator-(degrees rhs) const {
    return degrees(value_ - rhs.value_);
}

degrees degrees::operator*(double k) const {
    return degrees(value_ * k);
}

degrees degrees::operator/(double k) const {
    return degrees(value_ / k);
}

degrees& degrees::operator+=(degrees rhs) {
    value_ += rhs.value_;
    return *this;
}

degrees& degrees::operator-=(degrees rhs) {
    value_ -= rhs.value_;
    return *this;
}

degrees& degrees::operator*=(double k) {
    value_ *= k;
    return *this;
}

degrees& degrees::operator/=(double k) {
    value_ /= k;
    return *this;
}

double degrees::value() const {
    return value_;
}

degrees operator"" _deg(unsigned long long v) {
    return degrees(static_cast<double>(v));
}

degrees operator"" _deg(long double v) {
    return degrees(static_cast<double>(v));
}
