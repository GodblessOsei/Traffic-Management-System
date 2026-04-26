#include "LightController.h"
#include <stdexcept>

// Start with North/South green and East/West red.
LightController::LightController(std::map<Direction, TrafficLight*> trafficLights)
    : trafficLights(trafficLights), currentPhase(Phase::NS_GREEN), timer(0)
{

    // Check all directions directions are present before setting their traffic light colors,
    for (Direction d : {Direction::NORTH, Direction::SOUTH,
                        Direction::EAST,  Direction::WEST}) {
        auto it = this->trafficLights.find(d);
        if (it == this->trafficLights.end() || it->second == nullptr)
            throw std::invalid_argument(
                "LightController: a non-null TrafficLight must be provided for all four directions");
    }
    setLights(LightState::GREEN, LightState::RED);
}

// Set light states by directional group.
void LightController::setLights(LightState ns, LightState ew) {
    trafficLights[Direction::NORTH]->setState(ns);
    trafficLights[Direction::SOUTH]->setState(ns);
    trafficLights[Direction::EAST]->setState(ew);
    trafficLights[Direction::WEST]->setState(ew);
}

// Check if one direction currently has green.
bool LightController::isGreen(Direction direction) {
    auto it = trafficLights.find(direction);
    if (it != trafficLights.end()) {
        return it->second->getState() == LightState::GREEN;
    }
    return false; // Default to false if direction not found
}

// Advance phase timer and switch states when thresholds are reached.
void LightController::updateLights(float deltaTime) {
    timer += deltaTime;

    if (currentPhase == Phase::NS_GREEN && timer >= GREEN_DURATION) {
        // N/S: GREEN -> YELLOW, then enter NS_YELLOW
        setLights(LightState::YELLOW, LightState::RED);
        currentPhase = Phase::NS_YELLOW;
        timer = 0;
    } else if (currentPhase == Phase::NS_YELLOW && timer >= YELLOW_DURATION) {
        // N/S: YELLOW -> RED, E/W: RED -> GREEN, then enter EW_GREEN
        setLights(LightState::RED, LightState::GREEN);
        currentPhase = Phase::EW_GREEN;
        timer = 0;
    } else if (currentPhase == Phase::EW_GREEN && timer >= GREEN_DURATION) {
        // E/W: GREEN -> YELLOW, then enter EW_YELLOW
        setLights(LightState::RED, LightState::YELLOW);
        currentPhase = Phase::EW_YELLOW;
        timer = 0;
    } else if (currentPhase == Phase::EW_YELLOW && timer >= YELLOW_DURATION) {
        // E/W: YELLOW -> RED, N/S: RED -> GREEN, then enter NS_GREEN
        setLights(LightState::GREEN, LightState::RED);
        currentPhase = Phase::NS_GREEN;
        timer = 0;
    }
}

LightState LightController::getState(Direction direction) {
    // prevent derefrencing a nullptr if an direction has not trafficLight inserted.
    auto it = trafficLights.find(direction);
    if (it == trafficLights.end() || it->second == nullptr)
        throw std::invalid_argument("LightController: no traffic light registered for the requested direction");
    return it->second->getState();
}