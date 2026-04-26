#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
#include <stdexcept>

#include "../backend/Types.h"
#include "../backend/Car.h"
#include "../backend/Bus.h"
#include "../backend/Bike.h"
#include "../backend/Lane.h"
#include "../backend/Road.h"
#include "../backend/TrafficLight.h"
#include "../backend/LightController.h"
#include "../backend/Intersection.h"
#include "Config.h"
#include "Renderer.h"

int main()
{
try
{
    // ── Build lanes ──────────────────────────────────────────────────────────
    // Each road has a queue lane (cars wait here) and an arrival lane (cars
    // are placed here after crossing). Lanes are heap-allocated so Road and
    // Intersection can hold raw pointers without dangling-reference risk.

    Lane northQueue(1, true);   Lane northArrival(2,  false);
    Lane southQueue(3, true);   Lane southArrival(4,  false);
    Lane eastQueue (5, true);   Lane eastArrival (6,  false);
    Lane westQueue (7, true);   Lane westArrival (8,  false);

    // ── Build roads ──────────────────────────────────────────────────────────
    // One road per cardinal direction; each owns a queue/arrival lane pair.

    Road northRoad(1, Direction::NORTH, &northQueue, &northArrival);
    Road southRoad(2, Direction::SOUTH, &southQueue, &southArrival);
    Road eastRoad (3, Direction::EAST,  &eastQueue,  &eastArrival);
    Road westRoad (4, Direction::WEST,  &westQueue,  &westArrival);

    // ── Build traffic lights ─────────────────────────────────────────────────
    // N/S start GREEN; E/W start RED so the initial phase is consistent.

    TrafficLight northLight(1, LightState::GREEN, &northRoad);
    TrafficLight southLight(2, LightState::GREEN, &southRoad);
    TrafficLight eastLight (3, LightState::RED,   &eastRoad);
    TrafficLight westLight (4, LightState::RED,   &westRoad);

    // ── Build light controller ───────────────────────────────────────────────
    // Groups the four lights and drives the N/S ↔ E/W phase cycle.

    LightController lightController({
        {Direction::NORTH, &northLight},
        {Direction::SOUTH, &southLight},
        {Direction::EAST,  &eastLight},
        {Direction::WEST,  &westLight},
    });

    // ── Build intersection ───────────────────────────────────────────────────
    // Ties roads and the controller together under one update call.

    Intersection intersection(1, {
        {Direction::NORTH, &northRoad},
        {Direction::SOUTH, &southRoad},
        {Direction::EAST,  &eastRoad},
        {Direction::WEST,  &westRoad},
    }, &lightController);

    // Seed each approach lane with one of each vehicle type.
    intersection.addVehicle(Direction::NORTH, std::make_shared<Car> (2.0f, "red"));
    intersection.addVehicle(Direction::SOUTH, std::make_shared<Bus> (1.2f, "blue"));
    intersection.addVehicle(Direction::EAST,  std::make_shared<Bike>(3.5f, "green"));
    intersection.addVehicle(Direction::WEST,  std::make_shared<Car> (2.0f, "white"));

    // ── Open window ──────────────────────────────────────────────────────────
    // VideoMode takes (width, height); title shown in the title bar.

    sf::RenderWindow window(
        sf::VideoMode(sf::Vector2u(Config::WINDOW_W, Config::WINDOW_H)),
        "Traffic Management Simulation"
    );

    // Cap to monitor refresh rate — prevents burning the CPU in the event loop.
    window.setFramerateLimit(60);

    // ── Create renderer ──────────────────────────────────────────────────────
    Renderer renderer(window, intersection);

    // ── Clock and spawner state ──────────────────────────────────────────────
    sf::Clock clock;
    float spawnTimer = 0.f;
    int   colorIdx = 4, dirIdx = 0;  // start at index 4 so initial cars use 0–3
    static const char* colors[] = {
        "red", "blue", "green", "white", "orange", "purple", "cyan", "yellow"
    };
    static const Direction spawnDirs[] = {
        Direction::NORTH, Direction::EAST, Direction::SOUTH, Direction::WEST
    };

    // ── Main loop ────────────────────────────────────────────────────────────
    while (window.isOpen())
    {
        // -- Event handling --------------------------------------------------
        // Poll every queued OS event. Always handle at minimum the close event
        // so the window can be dismissed normally.
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // -- Delta time ------------------------------------------------------
        float dt = clock.restart().asSeconds();

        // -- Periodic car spawning -------------------------------------------
        spawnTimer += dt;
        if (spawnTimer >= Config::SPAWN_INTERVAL) {
            spawnTimer = 0.f;
            Direction d = spawnDirs[dirIdx % 4];
            const char* col = colors[colorIdx % 8];
            // Rotate through vehicle types: Car → Bus → Bike
            std::shared_ptr<Vehicle> v;
            switch (dirIdx % 3) {
                case 0: v = std::make_shared<Car> (2.0f, col); break;
                case 1: v = std::make_shared<Bus> (1.2f, col); break;
                default:v = std::make_shared<Bike>(3.5f, col); break;
            }
            intersection.addVehicle(d, v);
            dirIdx++;
            colorIdx++;
        }

        // -- Simulation update -----------------------------------------------
        intersection.update(dt);

        // -- Animate car positions -------------------------------------------
        renderer.updateVehiclePositions(dt);

        // -- Render ----------------------------------------------------------
        window.clear(sf::Color(30, 30, 30));
        renderer.draw();
        window.display();
    }

    return 0;
}
catch (const std::exception& e)
{
    // Any std::invalid_argument or std::logic_error thrown during setup
    // (bad speed value, missing road/light registration, etc.) lands here
    // instead of producing a cryptic crash with no message.
    std::cerr << "Fatal error: " << e.what() << '\n';
    return 1;
}
}
