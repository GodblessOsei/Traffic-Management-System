#pragma once
#include "Vehicle.h"


// A Larger vehicle. Dimensions are 40 px wide × 75 px tall (portrait orientation).
// Much slower than a Car
class Bus : public Vehicle
{
private:
    float speed;
    std::string colour;

public:
    Bus(float speed, std::string colour);

    float       getSpeed()  const override;
    std::string getColor()  const override;
    std::string getType()   const override; // Returns "Bus".
    float       getWidth()  const override; // 40 px — wider than a Car.
    float       getHeight() const override; // 75 px — longer than a Car.
};
