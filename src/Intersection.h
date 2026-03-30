#pragma once
#include "Types.h"
#include "Road.h"
#include "LightController.h"
#include <map>

using namespace std;

// Coordinates roads, lights, and car movement at one junction.
class Intersection
{
    private:
        int id;
        // Roads indexed by direction.
        map<Direction, Road*> roads;
        // Light scheduler for this intersection.
        LightController* lightController;

        void moveCars(Direction from, Direction to); // private helper

    public:
        // Build intersection with roads and one controller.
        Intersection(int id, map<Direction, Road*> roads, LightController* lightController);
        int getId();
        // Advance lights and move cars for this time step.
        void update(int deltaTime);
        void addCar(Direction direction, Car car); // adds car to a road's queue lane
        Road* getRoad(Direction direction);
        LightController* getLightController();
};
