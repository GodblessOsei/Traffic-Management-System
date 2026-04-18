// src/Types.h
#pragma once

#include<string>

// Cardinal directions used for roads and light lookup.
enum class Direction
{
    NORTH,
    SOUTH,
    EAST,
    WEST
};

// Possible states of a traffic signal.
enum class LightState
{
    RED,
    GREEN,
    YELLOW
};

// Signal timing constants in simulation seconds.
const int GREEN_DURATION = 10;
const int YELLOW_DURATION = 3;
const int RED_DURATION = 10;

inline std::string toString(LightState s) {
    switch(s) {
        case LightState::RED:    return "RED";
        case LightState::YELLOW: return "YELLOW";
        case LightState::GREEN:  return "GREEN";
    }
    return "";
}