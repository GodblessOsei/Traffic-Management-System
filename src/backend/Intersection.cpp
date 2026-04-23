#include "Intersection.h"

Intersection::Intersection(int id, map<Direction, Road*> roads, LightController* lightController) {
    this->id = id;
    this->roads = roads;
    this->lightController = lightController;
}

int Intersection::getId() { return id; }

Road* Intersection::getRoad(Direction direction) { return roads[direction]; }

LightController* Intersection::getLightController() { return lightController; }

Direction Intersection::opposite(Direction d) {
    switch (d) {
        case Direction::NORTH: return Direction::SOUTH;
        case Direction::SOUTH: return Direction::NORTH;
        case Direction::EAST:  return Direction::WEST;
        case Direction::WEST:  return Direction::EAST;
    }
    return d;
}

// Advance lights, then start one crossing per green direction that has a
// waiting car and an empty crossing slot. Crossings are completed externally
// (by the renderer) so that animation can finish before the car is committed.
void Intersection::update(float deltaTime) {
    lightController->updateLights(deltaTime);
    for (auto& [dir, road] : roads) {
        if (lightController->isGreen(dir) && !crossingCars[dir].has_value()) {
            Lane* queue = road->getQueueLane();
            if (!queue->isEmpty()) {
                crossingCars[dir] = queue->removeCar();
            }
        }
    }
}

void Intersection::addCar(Direction direction, Car car) {
    roads[direction]->getQueueLane()->addCar(car);
}

bool Intersection::hasCrossingCar(Direction from) const {
    auto it = crossingCars.find(from);
    return it != crossingCars.end() && it->second.has_value();
}

Car& Intersection::getCrossingCar(Direction from) {
    return *crossingCars[from];
}

void Intersection::completeCrossing(Direction from) {
    if (!crossingCars[from].has_value()) return;
    roads[opposite(from)]->getArrivalLane()->addCar(*crossingCars[from]);
    crossingCars[from].reset();
}
