#pragma once
#include "Types.h"
#include "Road.h"
#include "LightController.h"
#include "Vehicle.h"
#include <map>
#include <memory>

using namespace std;

// Central coordinator — owns the roads, delegates signal timing to LightController,
// a manages the two-phase crossing model:
//
//   Phase 1 (logic)    — update() pops a vehicle from a green queue lane into
//                        the crossing slot; the vehicle is "in transit" but has
//                        not yet moved on screen.
//   Phase 2 (animation) — the renderer animates the vehicle across the box and
//                          calls completeCrossing() when it reaches the far edge,
//                          which commits it to the opposite arrival lane.

class Intersection
{
private:
    int id;
    map<Direction, Road*> roads;
    LightController* lightController;

    // One crossing slot per direction — nullptr means no vehicle is currently
    // crossing from that arm. 
    map<Direction, shared_ptr<Vehicle>> crossingCars;

    // Returns the direction a vehicle ends up travelling after crossing.
    // NORTH → SOUTH, EAST → WEST, etc.
    static Direction opposite(Direction d);

public:
    Intersection(int id, map<Direction, Road*> roads, LightController* lightController);
    int getId();

    // Advances the light cycle, then for every green arm that has a waiting
    // vehicle and an empty crossing slot, pops the front vehicle into crossing.
    void update(float deltaTime);

    // Places a vehicle at the back of the queue lane for the given direction.
    void  addVehicle(Direction direction, shared_ptr<Vehicle> vehicle);
    Road* getRoad(Direction direction);
    LightController* getLightController();

    // Returns true if a vehicle is currently animating across the intersection box from direction 'from'.
    bool     hasCrossingCar(Direction from) const;
    
    // Returns a reference to the crossing vehicle so the renderer can update its position.
    Vehicle& getCrossingCar(Direction from);

    // Called by the renderer when the crossing animation completes.
    // Moves the shared_ptr from the crossing slot into the opposite arm's arrival lane.
    void completeCrossing(Direction from);
};
