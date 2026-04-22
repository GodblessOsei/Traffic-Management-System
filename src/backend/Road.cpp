#include "Road.h"

// Initialize road identity, direction, and lanes.
Road:: Road(int id, Direction direction, Lane* queueLane, Lane* arrivalLane){
    this->id = id;
    this->direction = direction;
    this->queueLane = queueLane;
    this->arrivalLane = arrivalLane;
}

// Return road identifier.
int Road:: getId(){
    return this->id;
}

// Return the signal queue lane.
Lane* Road:: getQueueLane(){
    return this->queueLane;
}

// Return the destination/arrival lane.
Lane* Road:: getArrivalLane(){
    return this->arrivalLane;
}

// Return road direction.
Direction Road:: getDirection(){
    return this->direction;
}

