#include "Bus.h"
#include <stdexcept>

Bus::Bus(float speed, std::string colour) : speed(speed), colour(colour) {
    if (speed <= 0.f)
        throw std::invalid_argument("Bus speed must be greater than zero (got "
                                    + std::to_string(speed) + ")");
}

float       Bus::getSpeed()  const { return speed; }
std::string Bus::getColor()  const { return colour; }
std::string Bus::getType()   const { return "Bus"; }
float       Bus::getWidth()  const { return 40.f; }
float       Bus::getHeight() const { return 75.f; }
