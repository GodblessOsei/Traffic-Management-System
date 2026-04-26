#include "Intersection.h"
#include <stdexcept>

Intersection::Intersection(int id, map<Direction, Road*> roads, LightController* lightController) {
    this->id = id;
    this->roads = roads;
    this->lightController = lightController;
}

int  Intersection::getId(){ return id; }
Road* Intersection::getRoad(Direction d) { //returns the road associated to a particular direction
    auto it = roads.find(d);
    if (it == roads.end() || it->second == nullptr) //prevent derefrencing a nullptr
        throw std::invalid_argument("Intersection: no road registered for the requested direction");
    return it->second;
}

LightController* Intersection::getLightController() { return lightController; }

Direction Intersection::opposite(Direction d) { //returns opposite road 
    switch (d) {
        case Direction::NORTH: return Direction::SOUTH;
        case Direction::SOUTH: return Direction::NORTH;
        case Direction::EAST:  return Direction::WEST;
        case Direction::WEST:  return Direction::EAST;
    }
    return d;
}

void Intersection::update(float deltaTime) {
    lightController->updateLights(deltaTime);
    for (auto& [dir, road] : roads) {
        if (lightController->isGreen(dir) && crossingCars[dir] == nullptr) { // Green and no car crossing 
            Lane* queue = road->getQueueLane();
            if (!queue->isEmpty())
                crossingCars[dir] = queue->removeVehicle();
        }
    }
}

void Intersection::addVehicle(Direction direction, shared_ptr<Vehicle> vehicle) {
    roads[direction]->getQueueLane()->addVehicle(vehicle);
}

bool Intersection::hasCrossingCar(Direction from) const {
    auto it = crossingCars.find(from);
    return it != crossingCars.end() && it->second != nullptr;
}

Vehicle& Intersection::getCrossingCar(Direction from) {
    if (!hasCrossingCar(from)) // check if a car is crossing to prevent derefrencing an nullptr
        throw std::logic_error("Intersection: getCrossingCar called but no vehicle is crossing from this direction");
    return *crossingCars[from];
}

void Intersection::completeCrossing(Direction from) {
    if (crossingCars[from] == nullptr) return;
    roads[opposite(from)]->getArrivalLane()->addVehicle(crossingCars[from]);
    crossingCars[from] = nullptr;
}
