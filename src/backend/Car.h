// src/Car.h
#pragma once
#include <iostream>

// Represents one vehicle in the simulation.
class Car
{

private:
    static int nextId;
    int id;
    float speed;
    std::string colour;
    float px = 0.f, py = 0.f;
    bool posSet = false;

public:
    bool canMove = false;

    Car(float speed, std::string colour);

    float getSpeed() const;
    std::string getColor() const;
    int getId() const;

    void setPos(float x, float y);
    float getPx() const;
    float getPy() const;
    bool isPosSet() const;
};
