// src/Types.h
#pragma once
#include <string>

// Cardinal directions — used as map keys so road/light lookups are direction-indexed.
enum class Direction { NORTH, SOUTH, EAST, WEST };

// Three-state signal model.
enum class LightState { RED, GREEN, YELLOW };

// Phase durations in simulation time units (seconds when dt is in seconds).
// A direction stays RED for the full opposing green + yellow cycle;
const int GREEN_DURATION  = 10;
const int YELLOW_DURATION = 3;

