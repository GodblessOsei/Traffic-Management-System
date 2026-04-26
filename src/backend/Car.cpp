#include "Car.h"
#include <stdexcept>

Car::Car(float speed, std::string colour) : speed(speed), colour(colour) {
    // A zero or negative speed means the vehicle will never reach its target
    // or will animate in the wrong direction 
    if (speed <= 0.f)
        throw std::invalid_argument("Car speed must be greater than zero (got "
                                    + std::to_string(speed) + ")");
}

float       Car::getSpeed()  const { return speed; }
std::string Car::getColor()  const { return colour; }
std::string Car::getType()   const { return "Car"; }
float       Car::getWidth()  const { return 30.f; }
float       Car::getHeight() const { return 50.f; }
