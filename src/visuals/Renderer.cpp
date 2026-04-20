#include "Renderer.h"
#include "./Config.h"
#include <SFML/Graphics.hpp>

// Initialize RendereWindow and Intersection
Renderer::Renderer(sf::RenderWindow& window, Intersection& intersection)
    : window(window), intersection(intersection)
{}

// ── Public entry point ───────────────────────────────────
void Renderer::draw() {
    drawRoads();
    drawLaneDivisions();
    drawTrafficLights();
    drawCars();
}

// ── Roads ────────────────────────────────────────────────
void Renderer::drawRoads() {
    sf::Color roadColor(80, 80, 80);

    // North road — vertical, above intersection
    sf::RectangleShape northRoad(sf::Vector2f(Config::ROAD_WIDTH, Config::INTERSECTION_Y));
    northRoad.setFillColor(roadColor);
    northRoad.setPosition(sf::Vector2f(Config::INTERSECTION_X, 0));
    window.draw(northRoad);

    // South road — vertical, below intersection
    sf::RectangleShape southRoad(sf::Vector2f(Config::ROAD_WIDTH,
        Config::WINDOW_H - Config::INTERSECTION_Y - Config::INTERSECTION_SIZE));
    southRoad.setFillColor(roadColor);
    southRoad.setPosition(sf::Vector2f(Config::INTERSECTION_X,
        Config::INTERSECTION_Y + Config::INTERSECTION_SIZE));
    window.draw(southRoad);

    // West road — horizontal, left of intersection
    sf::RectangleShape westRoad(sf::Vector2f(Config::INTERSECTION_X, Config::ROAD_WIDTH));
    westRoad.setFillColor(roadColor);
    westRoad.setPosition(sf::Vector2f(0, Config::INTERSECTION_Y));
    window.draw(westRoad);

    // East road — horizontal, right of intersection
    sf::RectangleShape eastRoad(sf::Vector2f(
        Config::WINDOW_W - Config::INTERSECTION_X - Config::INTERSECTION_SIZE,
        Config::ROAD_WIDTH));
    eastRoad.setFillColor(roadColor);
    eastRoad.setPosition(sf::Vector2f(Config::INTERSECTION_X + Config::INTERSECTION_SIZE,
        Config::INTERSECTION_Y));
    window.draw(eastRoad);

    // Intersection box
    sf::RectangleShape box(sf::Vector2f(Config::INTERSECTION_SIZE, Config::INTERSECTION_SIZE));
    box.setFillColor(sf::Color(60, 60, 60));
    box.setPosition(sf::Vector2f(Config::INTERSECTION_X, Config::INTERSECTION_Y));
    window.draw(box);
}

// ── Lane dividers ────────────────────────────────────────
void Renderer::drawLaneDivisions() {
    sf::Color divColor(255, 255, 0); // yellow dashes

    // Vertical divider — splits N/S road into two lanes
    sf::RectangleShape vDiv(sf::Vector2f(3, Config::WINDOW_H));
    vDiv.setFillColor(divColor);
    vDiv.setPosition(sf::Vector2f(Config::INTERSECTION_X + Config::LANE_WIDTH, 0));
    window.draw(vDiv);

    // Horizontal divider — splits E/W road into two lanes
    sf::RectangleShape hDiv(sf::Vector2f(Config::WINDOW_W, 3));
    hDiv.setFillColor(divColor);
    hDiv.setPosition(sf::Vector2f(0, Config::INTERSECTION_Y + Config::LANE_WIDTH));
    window.draw(hDiv);
}

// ── Traffic lights ───────────────────────────────────────
void Renderer::drawTrafficLights() {
    auto drawLight = [&](float x, float y, LightState state) {
        sf::Color col;
        if      (state == LightState::GREEN)  col = sf::Color(0, 200, 0);
        else if (state == LightState::YELLOW) col = sf::Color(255, 200, 0);
        else                                  col = sf::Color(200, 0, 0);

        sf::CircleShape light(12);
        light.setFillColor(col);
        light.setPosition(sf::Vector2f(x,y));
        window.draw(light);
    };

    LightController* lc = intersection.getLightController();

    drawLight(Config::INTERSECTION_X - 30,
              Config::INTERSECTION_Y - 30,
              lc->getState(Direction::NORTH));

    drawLight(Config::INTERSECTION_X + Config::INTERSECTION_SIZE + 10,
              Config::INTERSECTION_Y + Config::INTERSECTION_SIZE + 10,
              lc->getState(Direction::SOUTH));

    drawLight(Config::INTERSECTION_X + Config::INTERSECTION_SIZE + 10,
              Config::INTERSECTION_Y - 30,
              lc->getState(Direction::EAST));

    drawLight(Config::INTERSECTION_X - 30,
              Config::INTERSECTION_Y + Config::INTERSECTION_SIZE + 10,
              lc->getState(Direction::WEST));
}

// ── Cars ─────────────────────────────────────────────────
void Renderer::drawCars() {
    // Map each direction to spawn position and movement axis
    struct LaneInfo {
        float spawnX, spawnY;
        float targetX, targetY;
    };

    std::map<Direction, LaneInfo> laneInfo = {
        {Direction::NORTH, {Config::NORTH_SPAWN_X, Config::NORTH_SPAWN_Y,
                            Config::SOUTH_SPAWN_X, Config::SOUTH_SPAWN_Y}},
        {Direction::SOUTH, {Config::SOUTH_SPAWN_X, Config::SOUTH_SPAWN_Y,
                            Config::NORTH_SPAWN_X, Config::NORTH_SPAWN_Y}},
        {Direction::EAST,  {Config::EAST_SPAWN_X,  Config::EAST_SPAWN_Y,
                            Config::WEST_SPAWN_X,  Config::WEST_SPAWN_Y}},
        {Direction::WEST,  {Config::WEST_SPAWN_X,  Config::WEST_SPAWN_Y,
                            Config::EAST_SPAWN_X,  Config::EAST_SPAWN_Y}},
    };

    for (auto dir : {Direction::NORTH, Direction::SOUTH,
                     Direction::EAST,  Direction::WEST}) {
        Road* road = intersection.getRoad(dir);
        const auto& cars = road->getQueueLane()->getCars();
        const LaneInfo& info = laneInfo[dir];

        for (int i = 0; i < (int)cars.size(); i++) {
            sf::RectangleShape carShape(sf::Vector2f(Config::CAR_W, Config::CAR_H));
            carShape.setFillColor(sf::Color(220, 50, 50)); // red for now

            // Space cars along the lane based on queue position
            float offset = i * (Config::CAR_H + 5);
            float x = info.spawnX;
            float y = info.spawnY + offset;

            // Adjust offset direction per road
            if (dir == Direction::SOUTH) y = info.spawnY - offset;
            if (dir == Direction::EAST)  { x = info.spawnX - offset; y = info.spawnY; }
            if (dir == Direction::WEST)  { x = info.spawnX + offset; y = info.spawnY; }

            carShape.setPosition(sf::Vector2f(x, y));
            window.draw(carShape);
        }
    }
}