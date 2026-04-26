#include "Vehicle.h"


int Vehicle::nextId = 0;

// Increment nextId, as id to each vehicle constructed
Vehicle::Vehicle() : id(++nextId) {}

int   Vehicle::getId()    const { return id; }
float Vehicle::getPx()    const { return px; }
float Vehicle::getPy()    const { return py; }
bool  Vehicle::isPosSet() const { return posSet; }

// Sets posSet on every call so the renderer knows the vehicle has a valid position.
void Vehicle::setPos(float x, float y) { px = x; py = y; posSet = true; }
