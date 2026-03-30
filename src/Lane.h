#pragma once
#include <queue>
#include <string>
#include "Car.h"

class Road; // forward declaration of Road to avoid circular dependency

// Stores cars in FIFO order for a road segment.
class Lane
{

private:
    int id;
    // True when this lane is an incoming queue lane.
    bool QueueLane;
    // Cars waiting or arriving on this lane.
    std::queue<Car> cars;
    Road *road;    
public:
    // Build a lane and link it to a road.
    Lane(int id, Road *road, bool QueueLane);
    // Add a car to the back of the lane queue.
    void addCar(Car car);
    // Remove and return the front car.
    Car removeCar();
    // Check whether there are cars in the lane.
    bool isEmpty();
    int getId();
    // Tell whether this is a queue lane.
    bool isQueueLane();
};

