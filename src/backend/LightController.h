#pragma once
#include "Types.h"
#include "TrafficLight.h"
#include <map>

// Signal cycle states for grouped directions.
enum class Phase
{
    NS_GREEN,
    NS_YELLOW,
    EW_GREEN,
    EW_YELLOW,
};

// Manages all traffic lights at one intersection.
class LightController
{
    private:
        // One traffic light per direction.
        std::map<Direction, TrafficLight*> trafficLights;
        Phase currentPhase;
        float timer;
        void setLights(LightState ns, LightState ew);

    public:
        LightController(std::map<Direction, TrafficLight*> trafficLights);
        void updateLights(float deltaTime);
        bool isGreen(Direction direction);
        LightState getState(Direction direction);
};