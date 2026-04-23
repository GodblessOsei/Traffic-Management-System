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

// Remove and return the first car in the vector.
Car Lane::removeCar(){
    if (isEmpty())
        throw std::runtime_error("Cannot remove car from empty lane");
    Car car = cars.front();
    cars.erase(cars.begin());
    return car;

}

// Append a car to the back of the vector.
void Lane::addCar(Car car){
    this->cars.push_back(car);
}

// Check lane type.
bool Lane::isQueueLane(){
    return this->QueueLane;
}

const std::vector<Car>& Lane::getCars() const { return cars; }
std::vector<Car>& Lane::getCars() { return cars; }