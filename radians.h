#pragma once

#include <compare>
#include "degrees.h"
#include "const.h"

class radians {
public:
    explicit radians(double v);

    explicit operator double() const;
    explicit operator degrees() const;

    radians operator+(radians rhs) const;
    radians operator-(radians rhs) const;
    radians operator*(double k) const;
    radians operator/(double k) const;

    radians& operator+=(radians rhs);
    radians& operator-=(radians rhs);
    radians& operator*=(double k);
    radians& operator/=(double k);

    auto operator<=>(radians const&) const = default;

    double value() const;

private:
    double value_;
};

// user-defined literals
radians operator"" _rad(unsigned long long v);
radians operator"" _rad(long double v);
