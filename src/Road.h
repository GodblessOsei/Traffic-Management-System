#pragma once
#include "Types.h"
#include "Lane.h"

class Road
{
    private:
        int id;
        Direction direction;
        Lane* queueLane;
        Lane* arrivalLane;

    public:
        Road(int id, Direction direction, Lane* queueLane, Lane* arrivalLane);
        Lane* getQueueLane();
        Lane* getArrivalLane();
        Direction getDirection();
        int getId();
};