#include <SFML/Graphics.hpp>
#include <map>

#include "../backend/Types.h"
#include "../backend/Car.h"
#include "../backend/Lane.h"
#include "../backend/Road.h"
#include "../backend/TrafficLight.h"
#include "../backend/LightController.h"
#include "../backend/Intersection.h"
#include "Config.h"
#include "Renderer.h"

int main()
{
    // ── Build lanes ──────────────────────────────────────────────────────────
    // Each road has a queue lane (cars wait here) and an arrival lane (cars
    // are placed here after crossing). Lanes are heap-allocated so Road and
    // Intersection can hold raw pointers without dangling-reference risk.

    Lane northQueue(1, nullptr, true);   Lane northArrival(2,  nullptr, false);
    Lane southQueue(3, nullptr, true);   Lane southArrival(4,  nullptr, false);
    Lane eastQueue (5, nullptr, true);   Lane eastArrival (6,  nullptr, false);
    Lane westQueue (7, nullptr, true);   Lane westArrival (8,  nullptr, false);

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

    // Seed each approach lane with a few cars so the first frame is non-empty.
    intersection.addCar(Direction::NORTH, Car(2.0f, "red"));
    intersection.addCar(Direction::SOUTH, Car(2.0f, "blue"));
    intersection.addCar(Direction::EAST,  Car(2.0f, "green"));
    intersection.addCar(Direction::WEST,  Car(2.0f, "white"));

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

    // ── Clock for delta-time ─────────────────────────────────────────────────
    sf::Clock clock;

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

        // -- Simulation update -----------------------------------------------
        // Restart returns elapsed time and resets the clock atomically.
        // Convert to integer milliseconds — Intersection::update expects int.
        int dt = static_cast<int>(clock.restart().asSeconds()); //clock.restart().asSeconds();
        intersection.update(dt);

        // -- Render ----------------------------------------------------------
        window.clear(sf::Color(30, 30, 30));   // dark-grey background
        renderer.draw();                        // roads, dividers, lights, cars
        window.display();                       // flip front/back buffers
    }

    return 0;
}
