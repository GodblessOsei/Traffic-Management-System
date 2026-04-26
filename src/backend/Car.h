#pragma once
#include "Vehicle.h"

// A small car. Dimensions are 30 px wide × 50 px tall (portrait orientation).
class Car : public Vehicle
{
private:
    float speed;        // Pixels per second passed in at construction.
    std::string colour; // Colour name resolved into RGB at draw time by Renderer::parseCarColor.

public:
    Car(float speed, std::string colour);

    float       getSpeed()  const override;
    std::string getColor()  const override;
    std::string getType()   const override; // Returns "Car".
    float       getWidth()  const override; // 30 px — narrow side.
    float       getHeight() const override; // 50 px — long side.
};
