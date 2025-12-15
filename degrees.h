#pragma once

#include <compare>
#include "const.h"

class radians; // forward declaration

class degrees {
public:
    explicit degrees(double v);

    explicit operator double() const;
    explicit operator radians() const;

    degrees operator+(degrees rhs) const;
    degrees operator-(degrees rhs) const;
    degrees operator*(double k) const;
    degrees operator/(double k) const;

    degrees& operator+=(degrees rhs);
    degrees& operator-=(degrees rhs);
    degrees& operator*=(double k);
    degrees& operator/=(double k);

    auto operator<=>(degrees const&) const = default;

    double value() const;

private:
    double value_;
};

// user-defined literals
degrees operator"" _deg(unsigned long long v);
degrees operator"" _deg(long double v);
