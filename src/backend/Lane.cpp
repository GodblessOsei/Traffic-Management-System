#include "Lane.h"

Lane::Lane(int id, bool QueueLane)
    : id(id), QueueLane(QueueLane) {}

int  Lane::getId()        { return id; }
bool Lane::isEmpty()      { return vehicles.empty(); }
bool Lane::isQueueLane()  { return QueueLane; }

void Lane::addVehicle(std::shared_ptr<Vehicle> vehicle) {
    vehicles.push_back(vehicle);
}

std::shared_ptr<Vehicle> Lane::removeVehicle() {
    if (isEmpty())
        throw std::runtime_error("Cannot remove vehicle from empty lane");
    auto v = vehicles.front();
    vehicles.erase(vehicles.begin());
    return v;
}

const std::vector<std::shared_ptr<Vehicle>>& Lane::getVehicles() const { return vehicles; }
      std::vector<std::shared_ptr<Vehicle>>& Lane::getVehicles()       { return vehicles; }
