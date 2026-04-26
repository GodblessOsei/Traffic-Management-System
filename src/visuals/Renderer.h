#pragma once

#include "../backend/Intersection.h"
#include "../backend/Types.h"
#include <SFML/Graphics.hpp>
#include <string>

// Handles all visual output for one simulation frame.
// updateVehiclePositions(), writes screen coordinates to Vehicle::setPos() 
// so that the animation state lives on the vehicle and persists across
// frames without the renderer needing to store it separately.
class Renderer
{
private:
    sf::RenderWindow& window;
    Intersection&     intersection; // Read-only access to roads, lights, and vehicles.

    //Draw passes — called in order by draw() 
    void drawRoads();          // Asphalt rectangles and intersection box.
    void drawLaneDivisions();  // Dashed yellow centre lines
    void drawStopLines();      // White lines where vehicles wait for a green signal.
    void drawTrafficLights();  // Three-signal housings at each corner of the intersection.
    void drawVehicles();       // All vehicles across queue, crossing, and arrival lanes.

    // Helpers 

    // Draws one three-bulb traffic-light housing (pole + box + red/yellow/green circles).
    // inactive bulbs render as dark versions of their colour.
    void drawThreeSignalLight(float x, float y, LightState state);

    // Draws one vehicle at (x, y) travelling in dir.
    // vw = narrow side, vh = long side (portrait). Swapped internally for E/W vehicles
    void drawSingleVehicle(float x, float y, Direction dir, sf::Color bodyColor, float vw, float vh);

    // Maps a colour name string (e.g. "red", "cyan") to an sf::Color.
    // Returns a fallback pink-red if the name is not recognised.
    static sf::Color parseVehicleColor(const std::string& colorStr);

    // Returns the direction a vehicle faces after crossing the intersection.
    // Used so arrival-lane vehicles are drawn pointing away from the box.
    static Direction opposite(Direction d);

public:
    Renderer(sf::RenderWindow& window, Intersection& intersection);

    // Calls all draw passes in the correct  order.
    // Called between window.clear() and window.display().
    void draw();

    // Advances every vehicle's screen position toward its target for this frame.
    // Called BEFORE draw() so vehicles are in the right place when drawn.
    // Also triggers completeCrossing() when an animated vehicle reaches the far
    // edge of the intersection box, handing it off to the arrival lane.
    void updateVehiclePositions(float dt);
};
