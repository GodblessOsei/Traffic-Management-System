#pragma once
#include "Types.h"
#include "Road.h"

class TrafficLight
{
    friend class LightController; // to control setState()

    private:
        int id;
        LightState lightState;
        Road* road;
        void setState(LightState newState); // private — only accessible by LightController

    public:
        TrafficLight(int id, LightState lightState, Road* road);
        int getId();
        LightState getState();
        Road* getRoad();
};