#pragma once
#include "Types.h"
#include "Road.h"
#include "LightController.h"
#include <map>

using namespace std;

class Intersection
{
    private:
        int id;
        map<Direction, Road*> roads;
        LightController* lightController;

        void moveCars(Direction from, Direction to); // private helper

    public:
        Intersection(int id, map<Direction, Road*> roads, LightController* lightController);
        int getId();
        void update(int deltaTime);
        void addCar(Direction direction, Car car); // adds car to a road's queue lane
        Road* getRoad(Direction direction);
        LightController* getLightController();
};
