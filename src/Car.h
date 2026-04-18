// src/Car.h
#pragma once
#include <iostream>

// Represents one vehicle in the simulation.
class Car
{

private:
    // Unique car identifier.
    int nextid = 0;
    int id;
    // Car speed value (unit defined by simulation).
    int speed;
    // Visual color label for the car.
    std::string colour;

public:
    // Create a new car.
    Car(int speed, std::string colour);

    // Accessors.
    int getSpeed();
    std::string  getColor();
    int getId();
};
