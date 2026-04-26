#pragma once
#include "Types.h"
#include "Road.h"

// Represents one traffic light governing a single road arm.
// LightController controlls the lightState via a friend declaration.
class TrafficLight
{

    friend class LightController; // state machine driver

private:
    int        id;
    LightState lightState;
    Road*      road; // The road arm this light controls.

    // Only LightController controls the transition logic.
    void setState(LightState newState);

public:
    TrafficLight(int id, LightState lightState, Road* road);

    int        getId();
    LightState getState();
    Road*      getRoad();
};
