#include "Bike.h"
#include <stdexcept>

Bike::Bike(float speed, std::string colour) : speed(speed), colour(colour) {
    if (speed <= 0.f)
        throw std::invalid_argument("Bike speed must be greater than zero (got "
                                    + std::to_string(speed) + ")");
}

float       Bike::getSpeed()  const { return speed; }
std::string Bike::getColor()  const { return colour; }
std::string Bike::getType()   const { return "Bike"; }
float       Bike::getWidth()  const { return 16.f; }
float       Bike::getHeight() const { return 32.f; }
