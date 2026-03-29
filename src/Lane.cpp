#include "Lane.h"

Lane:: Lane(int id, Road *road, bool QueueLane){
    this->id = id;
    this->road = road;
    this->QueueLane = QueueLane;
}

int Lane::getId(){
    return this->id;
}

bool Lane::isEmpty(){
    return this->cars.empty();
}

Car Lane::removeCar(){
    if (isEmpty()) 
        throw std::runtime_error("Cannot remove car from empty lane");
    Car car = cars.front();
    cars.pop();
    return car;
    
}

void Lane::addCar(Car car){
    this->cars.push(car);
}

bool Lane::isQueueLane(){
    return this->QueueLane;
}