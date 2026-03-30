#include "Lane.h"

// Initialize lane metadata.
Lane:: Lane(int id, Road *road, bool QueueLane){
    this->id = id;
    this->road = road;
    this->QueueLane = QueueLane;
}

// Return lane identifier.
int Lane::getId(){
    return this->id;
}

// True when lane has no cars.
bool Lane::isEmpty(){
    return this->cars.empty();
}

// Pop one car from the front of the queue.
Car Lane::removeCar(){
    if (isEmpty()) 
        throw std::runtime_error("Cannot remove car from empty lane");
    Car car = cars.front();
    cars.pop();
    return car;
    
}

// Push one car to the back of the queue.
void Lane::addCar(Car car){
    this->cars.push(car);
}

// Check lane type.
bool Lane::isQueueLane(){
    return this->QueueLane;
}