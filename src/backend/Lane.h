#pragma once
#include <vector>
#include <memory>
#include <stdexcept>
#include "Vehicle.h"

// Ordered list of vehicles for one segment of a road.
// shared_ptr<Vehicle> is used so Lane can hold any Vehicle subtype (Car, Bus, Bike)
// Intersection keeps a second reference for crossing vehicles during their animation.
class Lane
{
private:
    int  id;
    bool QueueLane; // true = vehicles wait here for a green light; false = arrival/exit lane.

    // Vehicles are stored front-to-back: index 0 is closest to the stop line.
    std::vector<std::shared_ptr<Vehicle>> vehicles;

public:
    Lane(int id, bool QueueLane);

    // Appends to the back .
    void addVehicle(std::shared_ptr<Vehicle> vehicle);

    // Removes and returns the front vehicle 
    // Throws std::runtime_error if the lane is empty.
    std::shared_ptr<Vehicle> removeVehicle();

    bool isEmpty();
    int  getId();
    bool isQueueLane();

    // Two overloads so callers can safely change vehicle positions when needed
    
    const std::vector<std::shared_ptr<Vehicle>>& getVehicles() const;
          std::vector<std::shared_ptr<Vehicle>>& getVehicles();
};
