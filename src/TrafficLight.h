#pragma once
#include "Types.h"
#include "Road.h"

// Controls light state for one road direction.
class TrafficLight
{
    friend class LightController; // to control setState()

    private:
        int id;
        LightState lightState;
        // Road governed by this light.
        Road* road;
        void setState(LightState newState); // private — only accessible by TrafficLight and LightController

    public:
        // Create a traffic light with initial state.
        TrafficLight(int id, LightState lightState, Road* road);
        int getId();
        LightState getState();
        Road* getRoad();
};