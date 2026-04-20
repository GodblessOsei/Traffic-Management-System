#include <iostream>

#include "Car.h"
#include "Lane.h"
#include "Road.h"
#include "TrafficLight.h"
#include "LightController.h"
#include "Intersection.h"
#include "Types.h"

using namespace std;

int main()
{

    // Lanes
    Lane queueLaneNorth(1, nullptr, true);
    Lane arrivalLaneNorth(2, nullptr, false);
    Lane queueLaneSouth(3, nullptr, true);
    Lane arrivalLaneSouth(4, nullptr, false);
    Lane queueLaneEast(5, nullptr, true);
    Lane arrivalLaneEast(6, nullptr, false);
    Lane queueLaneWest(7, nullptr, true);
    Lane arrivalLaneWest(8, nullptr, false);

    // Roads
    Road northRoad(1,Direction::NORTH, &queueLaneNorth, &arrivalLaneNorth);
    Road southRoad(2,Direction::SOUTH, &queueLaneSouth, &arrivalLaneSouth);
    Road eastRoad(3,Direction::EAST, &queueLaneEast, &arrivalLaneEast);
    Road westRoad(4,Direction::WEST, &queueLaneWest, &arrivalLaneWest);

    // Traffic Lights
    TrafficLight northLight(1, LightState::RED, &northRoad);
    TrafficLight southLight(2, LightState::RED, &southRoad);
    TrafficLight eastLight(3, LightState::RED, &eastRoad);
    TrafficLight westLight(4, LightState::RED, &westRoad);

    // Light Controller
    map<Direction, TrafficLight*> trafficLights = {
        {Direction::NORTH, &northLight},
        {Direction::SOUTH, &southLight},
        {Direction::EAST, &eastLight},
        {Direction::WEST, &westLight}
    };
    LightController lightController(trafficLights);

    // Intersection
    map<Direction, Road*> roads = {
        {Direction::NORTH, &northRoad},
        {Direction::SOUTH, &southRoad},
        {Direction::EAST, &eastRoad},
        {Direction::WEST, &westRoad}
    };

    Intersection intersection(1, roads, &lightController);

    // Add cars to Queueing Lane
    Car car1(60, "Red");
    Car car2(50, "Blue"); 
    Car car3(70, "Green");
    Car car4(55, "Yellow");

    intersection.addCar(Direction::NORTH, car1);
    intersection.addCar(Direction::SOUTH, car2);
    intersection.addCar(Direction::EAST, car3);
    intersection.addCar(Direction::WEST, car4);

    // Run a loop for 30 ticks with deltaTime = 1
    for (int i = 0; i < 30; i++) {
        intersection.update(1.0f); // 1 second per tick
        cout << "Tick " << i+1
         << " | N:" << toString(northLight.getState())
         << " | S:" << toString(southLight.getState())
         << " | E:" << toString(eastLight.getState())
         << " | W:" << toString(westLight.getState())
         << endl;
    }

    return 0;
}
