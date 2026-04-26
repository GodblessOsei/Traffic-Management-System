#pragma once
#include "Types.h"
#include "Lane.h"

// Models one arm of the intersection 
// Each road owns two lanes:
//   queueLane   — vehicles approach and wait here for a green signal.
//   arrivalLane — vehicles are placed here after crossing the box and continue
//                 moving until they despawn off-screen.
class Road
{
private:
    int       id;
    Direction direction; // Direction vehicles travel when entering from this arm.
    Lane*     queueLane;
    Lane*     arrivalLane;

public:
    Road(int id, Direction direction, Lane* queueLane, Lane* arrivalLane);

    int       getId();
    Direction getDirection();
    Lane*     getQueueLane();
    Lane*     getArrivalLane();
};
