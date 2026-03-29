#include "Intersection.h"

Intersection:: Intersection(int id, map<Direction, Road*> roads, LightController* lightController){
    this->id = id;
    this->roads = roads;
    this->lightController = lightController;
}

int Intersection::getId(){
    return this->id;
}

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

void Intersection::addCar(Direction direction, Car car){
    roads[direction]->getQueueLane()->addCar(car);
}

Road* Intersection::getRoad(Direction direction){
    return roads[direction];
}

LightController* Intersection::getLightController(){
    return this->lightController;
}

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

