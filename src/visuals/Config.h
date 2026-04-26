#pragma once

// All rendering constants.
// constexpr for compile-time substitution
namespace Config {

    // Window size
    constexpr float WINDOW_W = 800.f; // width in pixels.
    constexpr float WINDOW_H = 800.f; // height in pixels.

    // Intersection box
    // Positioned at the top-left corner of the window
    constexpr float INTERSECTION_X    = 300.f;
    constexpr float INTERSECTION_Y    = 300.f;
    constexpr float INTERSECTION_SIZE = 200.f;

    // Road and lane widths
    // ROAD_WIDTH = 2 × LANE_WIDTH — each road has two lanes side by side.
    constexpr float ROAD_WIDTH = 200.f;
    constexpr float LANE_WIDTH = 100.f; // Width of one lane; also used for stop-line length.

    // Stop-line coordinates
    // Each stop line sits at the edge of the intersection box.
    // NORTH/SOUTH are y-values; EAST/WEST are x-values.
    constexpr float NORTH_STOP = 300.f; // y: top edge of intersection box.
    constexpr float SOUTH_STOP = 500.f; // y: bottom edge of intersection box.
    constexpr float EAST_STOP  = 500.f; // x: right edge of intersection box.
    constexpr float WEST_STOP  = 300.f; // x: left edge of intersection box.

    // Vehicle spawn positions
    constexpr float NORTH_SPAWN_X = 350.f; // Left lane of north road (approaching south).
    constexpr float NORTH_SPAWN_Y =  50.f;
    constexpr float SOUTH_SPAWN_X = 450.f; // Right lane of south road (approaching north).
    constexpr float SOUTH_SPAWN_Y = 750.f;
    constexpr float EAST_SPAWN_X  = 750.f;
    constexpr float EAST_SPAWN_Y  = 350.f; // Top lane of east road (approaching west).
    constexpr float WEST_SPAWN_X  =  50.f;
    constexpr float WEST_SPAWN_Y  = 450.f; // Bottom lane of west road (approaching east.

    // Animation speed
    // Global cap on how many pixels a vehicle moves per second.
    // Individual vehicle speed (Vehicle::getSpeed) scales against this value.
    constexpr float CAR_SPEED = 100.0f;

    // Arrival lane centres
    // X or Y coordinate of the lane a vehicle is placed in after crossing.
    // Offset by one LANE_WIDTH from the queue lane so exiting vehicles use
    // the correct half of the road (oncoming traffic stays in its own lane).
    constexpr float NORTH_ARRIVAL_X = 450.f; // Right lane, exiting toward south.
    constexpr float SOUTH_ARRIVAL_X = 350.f; // Left lane, exiting toward north.
    constexpr float EAST_ARRIVAL_Y  = 450.f; // Bottom lane, exiting toward west.
    constexpr float WEST_ARRIVAL_Y  = 350.f; // Top lane, exiting toward east.

    // Spawner interval
    // A new vehicle is added to the next approach lane every SPAWN_INTERVAL seconds.
    constexpr float SPAWN_INTERVAL = 3.5f;
}
