// src/Car.h
#pragma once
#include <iostream>

// Represents one vehicle in the simulation.
class Car
{

private:
    // Unique car identifier.
    static int nextId;
    int id;
    // Car speed value (unit defined by simulation).
    float speed;
    // Visual color label for the car.
    std::string colour;

   


public:
    bool canMove = false; //public because external factors determine if a car can move

    // Create a new car.
    Car(float speed, std::string colour);

    // Accessors.
    float getSpeed();
    std::string  getColor();
    int getId();
};
