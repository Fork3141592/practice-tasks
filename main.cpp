#include <iostream>
#include "degrees.h"
#include "radians.h"

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
