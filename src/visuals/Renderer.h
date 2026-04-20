#pragma once

#include "../Intersection.h"

namespace sf
{
    class RenderWindow;
}

class Renderer
{
private:
    sf::RenderWindow &window;
    Intersection &intersection;
    void drawRoads();
    void drawLaneDivisions();
    void drawTrafficLights();
    void drawCars();

public:
    Renderer(sf::RenderWindow& window, Intersection& intersection);
    void draw();
};
