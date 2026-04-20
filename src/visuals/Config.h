#pragma once

namespace Config {
    // Window
    constexpr int WINDOW_W = 800;
    constexpr int WINDOW_H = 800;

    // Intersection box
    constexpr int INTERSECTION_X = 300;
    constexpr int INTERSECTION_Y = 300;
    constexpr int INTERSECTION_SIZE = 200;

    // Road width and lane width
    constexpr int ROAD_WIDTH  = 200;
    constexpr int LANE_WIDTH  = 100;

    // Stop lines
    constexpr int NORTH_STOP = 300;  // y value
    constexpr int SOUTH_STOP = 500;  // y value
    constexpr int EAST_STOP  = 500;  // x value
    constexpr int WEST_STOP  = 300;  // x value

    // Car spawn positions (center of approaching lane)
    constexpr float NORTH_SPAWN_X = 350;
    constexpr float NORTH_SPAWN_Y = 50;
    constexpr float SOUTH_SPAWN_X = 450;
    constexpr float SOUTH_SPAWN_Y = 750;
    constexpr float EAST_SPAWN_X  = 750;
    constexpr float EAST_SPAWN_Y  = 350;
    constexpr float WEST_SPAWN_X  = 50;
    constexpr float WEST_SPAWN_Y  = 450;

    // Car size
    constexpr float CAR_W = 30;
    constexpr float CAR_H = 50;

    // Car speed (pixels per second)
    constexpr float CAR_SPEED = 100.0f;
}