// src/Types.h
#pragma once

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