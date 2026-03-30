#include "Intersection.h"

// Initialize intersection identity, roads, and controller.
Intersection:: Intersection(int id, map<Direction, Road*> roads, LightController* lightController){
    this->id = id;
    this->roads = roads;
    this->lightController = lightController;
}

// Return intersection identifier.
int Intersection::getId(){
    return this->id;
}

// Update lights, then move cars from green-light roads.
void Intersection::update(int deltaTime){
    lightController->updateLights(deltaTime);
    for (const auto& pair : roads) { // Check each road's traffic light
        Direction dir = pair.first;
        Road* road = pair.second;
        if (lightController->isGreen(dir)) {
            Direction oppositeDir;
            switch (dir) { // Determine opposite direction
                case Direction::NORTH: oppositeDir = Direction::SOUTH; break;
                case Direction::SOUTH: oppositeDir = Direction::NORTH; break;
                case Direction::EAST: oppositeDir = Direction::WEST; break;
                case Direction::WEST: oppositeDir = Direction::EAST; break;
            }
            moveCars(dir, oppositeDir);
        }
    }
}

// Add a car to a specific direction's queue lane.
void Intersection::addCar(Direction direction, Car car){
    roads[direction]->getQueueLane()->addCar(car);
}

// Return road by direction.
Road* Intersection::getRoad(Direction direction){
    return roads[direction];
}

// Return the light controller assigned to this intersection.
LightController* Intersection::getLightController(){
    return this->lightController;
}

// Move one car from a source queue lane to a destination arrival lane.
void Intersection::moveCars(Direction from, Direction to){
    Road* fromRoad = roads[from];
    Road* toRoad = roads[to];
    Lane* fromLane = fromRoad->getQueueLane();
    Lane* toLane = toRoad->getArrivalLane();

    if (!fromLane->isEmpty()) {
        Car car = fromLane->removeCar();
        toLane->addCar(car);
    }
}

