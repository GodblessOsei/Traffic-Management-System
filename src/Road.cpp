#include "Road.h"

Road:: Road(int id, Direction direction, Lane* queueLane, Lane* arrivalLane){
    this->id = id;
    this->direction = direction;
    this->queueLane = queueLane;
    this->arrivalLane = arrivalLane;
}

int Road:: getId(){
    return this->id;
}

Lane* Road:: getQueueLane(){
    return this->queueLane;
}

Lane* Road:: getArrivalLane(){
    return this->arrivalLane;
}

Direction Road:: getDirection(){
    return this->direction;
}

