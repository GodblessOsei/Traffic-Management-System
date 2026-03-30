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
        // Elapsed time inside the current phase.
        int timer;
        // Apply states to N/S and E/W groups.
        void setLights(LightState ns, LightState ew);

    public:
        LightController(std::map<Direction, TrafficLight*> trafficLights);
        void updateLights(int deltaTime);
        bool isGreen(Direction direction);
};