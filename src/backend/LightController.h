#pragma once
#include "Types.h"
#include "TrafficLight.h"
#include <map>

// Four-state phase cycle.
// Transitions: GREEN → YELLOW → (opposing) GREEN to prevent
// both axes being green at the same time.
enum class Phase
{
    NS_GREEN,   // North and South have right-of-way.
    NS_YELLOW,  // North/South clearing; East/West still red.
    EW_GREEN,   // East and West have right-of-way.
    EW_YELLOW,  // East/West clearing; North/South still red.
};

// Drives the signal cycle for all four lights at one intersection.
// Lights are grouped by axis (N/S together, E/W together) because opposing
// directions always share the same state — vehicles travel in both directions
// on the same axis simultaneously.
class LightController
{
private:
    std::map<Direction, TrafficLight*> trafficLights; // One light per arm.
    Phase currentPhase;
    float timer; // Accumulates deltaTime; resets to 0 on every phase transition.

    // Applies one state to the N/S pair and another to the E/W pair in one call.
    void setLights(LightState ns, LightState ew);

public:
    // Constructor  calls setLights to put N/S green and E/W red
    // so the simulation starts in a valid state on frame 0.
    LightController(std::map<Direction, TrafficLight*> trafficLights);

    // Accumulates dt and advances the phase when the current duration expires.
    void updateLights(float deltaTime);

    // Returns true if the given direction currently shows GREEN.
    // Used by Intersection::update to decide which queue lanes is up.
    bool isGreen(Direction direction);

    // Returns the raw LightState for a direction for the Renderer
    LightState getState(Direction direction);
};
