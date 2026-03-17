#pragma once
#include "Types.h"
#include "Road.h"

class TrafficLight
{
    private:
        int id;
        LightState lightState;
        int timer; // Timer to manage state changes
        Road* road; // Associated road for the traffic light

    public:
        TrafficLight(int id, LightState lightState, Road* road);
        int getId();
        LightState getState();
        void update( int deltaTime); // Update the traffic light state based on the timer
        Road* getRoad();
      
};
