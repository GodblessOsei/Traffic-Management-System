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

// Return the car identifier.
int Car::getId(){
    return this->id;
}

// Return the current speed value.
float Car::getSpeed(){
    return this->speed;
}

// Return the car color label.
std::string Car::getColor(){
    return this->colour;
}

