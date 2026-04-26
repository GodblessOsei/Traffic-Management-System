#pragma once
#include "Vehicle.h"

// Lightweight vehicle type. Dimension are 16 wide x 32 px tall
class Bike : public Vehicle
{
private:
    float speed;
    std::string colour;

public:
    Bike(float speed, std::string colour);

    float       getSpeed()  const override;
    std::string getColor()  const override;
    std::string getType()   const override; // Returns "Bike".
    float       getWidth()  const override; // 16 px — narrowest vehicle.
    float       getHeight() const override; // 32 px — shortest vehicle.
};
