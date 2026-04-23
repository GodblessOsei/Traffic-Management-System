#pragma once
#include "Types.h"
#include "Road.h"
#include "LightController.h"
#include <map>
#include <optional>

using namespace std;

// Coordinates roads, lights, and car movement at one junction.
class Intersection
{
    private:
        int id;
        map<Direction, Road*> roads;
        LightController* lightController;
        // One crossing slot per direction: car is in transit across the box.
        map<Direction, optional<Car>> crossingCars;

        static Direction opposite(Direction d);

    public:
        Intersection(int id, map<Direction, Road*> roads, LightController* lightController);
        int getId();

        // Advance lights and start crossings for any green direction with a
        // waiting car and an empty crossing slot. Does NOT complete crossings —
        // the renderer calls completeCrossing() when the animation finishes.
        void update(float deltaTime);

        void addCar(Direction direction, Car car);
        Road* getRoad(Direction direction);
        LightController* getLightController();

        // Crossing-car accessors used by the renderer.
        bool hasCrossingCar(Direction from) const;
        Car& getCrossingCar(Direction from);
        // Called by the renderer when the crossing animation finishes.
        // Moves the car from the crossing slot into the opposite arrival lane.
        void completeCrossing(Direction from);
};
