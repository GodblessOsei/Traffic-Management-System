#pragma once
#include <queue>
#include <string>
#include "Car.h"

class Road; // forward declaration of Road to avoid circular dependency

class Lane
{

private:
    int id;
    bool isQueueLane;
    std::queue<Car> cars;
    Road *road;    
public:
    Lane(int id, Road *road, bool isQueueLane);
    void addCar(Car car);
    Car removeCar();
    bool isEmpty();
    int getId();
    bool getQueueLane();
};

