#pragma once
#include "Types.h"
#include "TrafficLight.h"
#include <map>

enum class Phase
{
    NS_GREEN,
    NS_YELLOW,
    EW_GREEN,
    EW_YELLOW,
};

class LightController
{
    private:
        std::map<Direction, TrafficLight*> trafficLights;
        Phase currentPhase;
        int timer;
        void setLights(LightState ns, LightState ew);

    public:
        LightController(std::map<Direction, TrafficLight*> trafficLights);
        void updateLights(int deltaTime);
        bool isGreen(Direction direction);
};