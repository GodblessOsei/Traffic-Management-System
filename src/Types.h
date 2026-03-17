// src/Types.h
#pragma once
enum class Direction
{
    NORTH,
    SOUTH,
    EAST,
    WEST
};

enum class LightState
{
    RED,
    GREEN,
    YELLOW
};

const int GREEN_DURATION = 10;
const int YELLOW_DURATION = 3;
const int RED_DURATION = 10;