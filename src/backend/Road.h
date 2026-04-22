#pragma once
#include "Types.h"
#include "Lane.h"

// Connects one travel direction with its lanes.
class Road
{
    private:
        int id;
        // Orientation of this road at the intersection.
        Direction direction;
        // Lane where cars wait at the signal.
        Lane* queueLane;
        // Lane where cars arrive after crossing.
        Lane* arrivalLane;

    public:
        // Create a road with queue and arrival lanes.
        Road(int id, Direction direction, Lane* queueLane, Lane* arrivalLane);
        Lane* getQueueLane();
        Lane* getArrivalLane();
        Direction getDirection();
        int getId();
};