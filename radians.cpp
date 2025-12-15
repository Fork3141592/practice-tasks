#include "radians.h"

radians::radians(double v) : value_(v) {}

radians::operator double() const {
    return value_;
}

radians::operator degrees() const {
    return degrees(value_ * 180.0 / pi);
}

radians radians::operator+(radians rhs) const {
    return radians(value_ + rhs.value_);
}

radians radians::operator-(radians rhs) const {
    return radians(value_ - rhs.value_);
}

radians radians::operator*(double k) const {
    return radians(value_ * k);
}

radians radians::operator/(double k) const {
    return radians(value_ / k);
}

radians& radians::operator+=(radians rhs) {
    value_ += rhs.value_;
    return *this;
}

radians& radians::operator-=(radians rhs) {
    value_ -= rhs.value_;
    return *this;
}

radians& radians::operator*=(double k) {
    value_ *= k;
    return *this;
}

radians& radians::operator/=(double k) {
    value_ /= k;
    return *this;
}

double radians::value() const {
    return value_;
}

radians operator"" _rad(unsigned long long v) {
    return radians(static_cast<double>(v));
}

radians operator"" _rad(long double v) {
    return radians(static_cast<double>(v));
}
