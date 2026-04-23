#include "Car.h"
#include <string>
#include<iostream>



// Initialize all car properties.

int Car:: nextId = 0;

Car::Car(float speed, std::string colour){
    this->id = ++nextId;
    this->speed = speed;
    this->colour = colour;
}

int Car::getId() const { return id; }
float Car::getSpeed() const { return speed; }
std::string Car::getColor() const { return colour; }

void Car::setPos(float x, float y) { px = x; py = y; posSet = true; }
float Car::getPx() const { return px; }
float Car::getPy() const { return py; }
bool Car::isPosSet() const { return posSet; }

