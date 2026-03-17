#include "Car.h"
#include <string>
#include<iostream>

Car::Car(int id, int speed, std::string colour){
    this->id = id;
    this->speed = speed;
    this->colour = colour;
}

int Car::getId(){
    return this->id;
}

int Car::getSpeed(){
    return this->speed;
}

std::string Car::getColor(){
    return this->colour;
}

