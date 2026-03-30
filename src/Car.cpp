#include "Car.h"
#include <string>
#include<iostream>

// Initialize all car properties.
Car::Car(int id, int speed, std::string colour){
    this->id = id;
    this->speed = speed;
    this->colour = colour;
}

// Return the car identifier.
int Car::getId(){
    return this->id;
}

// Return the current speed value.
int Car::getSpeed(){
    return this->speed;
}

// Return the car color label.
std::string Car::getColor(){
    return this->colour;
}

