#pragma once

#include "../backend/Intersection.h"
#include "../backend/Types.h"
#include <SFML/Graphics.hpp>
#include <string>

class Renderer
{
private:
    sf::RenderWindow& window;
    Intersection&     intersection;

    void drawRoads();
    void drawLaneDivisions();
    void drawStopLines();
    void drawTrafficLights();
    void drawCars();

    void drawThreeSignalLight(float x, float y, LightState state);
    void drawSingleCar(float x, float y, Direction dir, sf::Color bodyColor);

    static sf::Color parseCarColor(const std::string& colorStr);
    static Direction opposite(Direction d);

public:
    Renderer(sf::RenderWindow& window, Intersection& intersection);
    void draw();
    void updateCarPositions(float dt);
};
