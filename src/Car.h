// src/Car.h
#pragma once
#include <iostream>

class Car
{

private:
    int id;
    int speed;
    std::string colour;

public:
    // constructor
    Car(int id, int speed, std::string colour);

    // getters
    int getSpeed();
    std::string  getColor();
    int getId();
};
