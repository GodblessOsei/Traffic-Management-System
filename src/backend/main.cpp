#include <cassert>
#include <iostream>
#include <memory>
#include <stdexcept>

#include "Car.h"
#include "Lane.h"
#include "Road.h"
#include "TrafficLight.h"
#include "LightController.h"
#include "Intersection.h"
#include "Types.h"

using namespace std;

static int passed = 0;
static int failed = 0;

#define TEST(name, expr)                                           \
    do {                                                           \
        if (expr) { cout << "[PASS] " name "\n"; ++passed; }      \
        else       { cout << "[FAIL] " name "\n"; ++failed; }      \
    } while (false)

// ── Car ──────────────────────────────────────────────────────────────────────
void test_car() {
    Car c1(80.f, "red");
    Car c2(60.f, "blue");

    TEST("Car getColor returns color",  c1.getColor() == "red");
    TEST("Car getSpeed returns speed",  c1.getSpeed() == 80.f);
    TEST("Car ids are unique",          c1.getId() != c2.getId());
    TEST("Car ids are sequential",      c2.getId() == c1.getId() + 1);

    TEST("Car isPosSet false by default", !c1.isPosSet());
    c1.setPos(100.f, 200.f);
    TEST("Car isPosSet true after setPos", c1.isPosSet());
    TEST("Car getPx returns set value",    c1.getPx() == 100.f);
    TEST("Car getPy returns set value",    c1.getPy() == 200.f);
}

// ── Lane ─────────────────────────────────────────────────────────────────────
void test_lane() {
    Lane lane(1, true);

    TEST("Lane isEmpty on creation", lane.isEmpty());

    auto a = make_shared<Car>(10.f, "green");
    auto b = make_shared<Car>(20.f, "yellow");
    lane.addVehicle(a);
    lane.addVehicle(b);

    TEST("Lane not empty after addVehicle",       !lane.isEmpty());
    TEST("Lane getVehicles has 2 entries",          lane.getVehicles().size() == 2);

    auto front = lane.removeVehicle();
    TEST("removeVehicle returns first vehicle (FIFO)", front->getId() == a->getId());
    TEST("Lane has 1 vehicle after remove",            lane.getVehicles().size() == 1);

    lane.removeVehicle();
    TEST("Lane empty after removing all", lane.isEmpty());

    bool threw = false;
    try { lane.removeVehicle(); } catch (const runtime_error&) { threw = true; }
    TEST("removeVehicle on empty lane throws", threw);
}

// ── LightController ──────────────────────────────────────────────────────────
void test_light_controller() {
    Lane ql1(1,true), al1(2,false);
    Lane ql2(3,true), al2(4,false);
    Lane ql3(5,true), al3(6,false);
    Lane ql4(7,true), al4(8,false);

    Road rN(1,Direction::NORTH,&ql1,&al1);
    Road rS(2,Direction::SOUTH,&ql2,&al2);
    Road rE(3,Direction::EAST, &ql3,&al3);
    Road rW(4,Direction::WEST, &ql4,&al4);

    TrafficLight lN(1,LightState::GREEN,&rN);
    TrafficLight lS(2,LightState::GREEN,&rS);
    TrafficLight lE(3,LightState::RED,  &rE);
    TrafficLight lW(4,LightState::RED,  &rW);

    LightController lc({
        {Direction::NORTH,&lN},{Direction::SOUTH,&lS},
        {Direction::EAST, &lE},{Direction::WEST, &lW}
    });

    TEST("Initial N/S phase is GREEN", lc.getState(Direction::NORTH) == LightState::GREEN);
    TEST("Initial E/W phase is RED",   lc.getState(Direction::EAST)  == LightState::RED);
    TEST("isGreen NORTH at start",     lc.isGreen(Direction::NORTH));
    TEST("isGreen EAST false at start",!lc.isGreen(Direction::EAST));

    // Advance past NS_GREEN (GREEN_DURATION = 10s)
    lc.updateLights(10.f);
    TEST("N/S transitions to YELLOW after GREEN_DURATION",
         lc.getState(Direction::NORTH) == LightState::YELLOW);
    TEST("E/W stays RED during NS_YELLOW",
         lc.getState(Direction::EAST)  == LightState::RED);

    // Advance past NS_YELLOW (YELLOW_DURATION = 3s)
    lc.updateLights(3.f);
    TEST("E/W turns GREEN after NS_YELLOW elapses",
         lc.getState(Direction::EAST)  == LightState::GREEN);
    TEST("N/S turns RED after NS_YELLOW elapses",
         lc.getState(Direction::NORTH) == LightState::RED);
}

// ── Intersection ─────────────────────────────────────────────────────────────
void test_intersection() {
    Lane ql1(1,true), al1(2,false);
    Lane ql2(3,true), al2(4,false);
    Lane ql3(5,true), al3(6,false);
    Lane ql4(7,true), al4(8,false);

    Road rN(1,Direction::NORTH,&ql1,&al1);
    Road rS(2,Direction::SOUTH,&ql2,&al2);
    Road rE(3,Direction::EAST, &ql3,&al3);
    Road rW(4,Direction::WEST, &ql4,&al4);

    TrafficLight lN(1,LightState::GREEN,&rN);
    TrafficLight lS(2,LightState::GREEN,&rS);
    TrafficLight lE(3,LightState::RED,  &rE);
    TrafficLight lW(4,LightState::RED,  &rW);

    LightController lc({
        {Direction::NORTH,&lN},{Direction::SOUTH,&lS},
        {Direction::EAST, &lE},{Direction::WEST, &lW}
    });

    Intersection ix(1,{
        {Direction::NORTH,&rN},{Direction::SOUTH,&rS},
        {Direction::EAST, &rE},{Direction::WEST, &rW}
    },&lc);

    ix.addVehicle(Direction::NORTH, make_shared<Car>(1.f,"white"));
    ix.addVehicle(Direction::NORTH, make_shared<Car>(1.f,"black"));
    ix.addVehicle(Direction::EAST,  make_shared<Car>(1.f,"cyan"));

    TEST("NORTH queue has 2 vehicles after addVehicle",
         ix.getRoad(Direction::NORTH)->getQueueLane()->getVehicles().size() == 2);
    TEST("EAST queue has 1 vehicle after addVehicle",
         ix.getRoad(Direction::EAST)->getQueueLane()->getVehicles().size() == 1);

    // Phase 1: update starts the crossing — vehicle leaves queue, enters crossing slot
    ix.update(1.f);

    TEST("NORTH queue shrinks by 1 when crossing starts",
         ix.getRoad(Direction::NORTH)->getQueueLane()->getVehicles().size() == 1);
    TEST("NORTH has a crossing vehicle in transit",
         ix.hasCrossingCar(Direction::NORTH));
    TEST("SOUTH arrival is still empty while vehicle is crossing",
         ix.getRoad(Direction::SOUTH)->getArrivalLane()->getVehicles().size() == 0);
    TEST("EAST queue unchanged — light is red, no crossing started",
         ix.getRoad(Direction::EAST)->getQueueLane()->getVehicles().size() == 1);
    TEST("EAST has no crossing vehicle (red light)",
         !ix.hasCrossingCar(Direction::EAST));

    // Phase 2: renderer calls completeCrossing when animation finishes
    ix.completeCrossing(Direction::NORTH);

    TEST("SOUTH arrival gains 1 vehicle after completeCrossing",
         ix.getRoad(Direction::SOUTH)->getArrivalLane()->getVehicles().size() == 1);
    TEST("NORTH crossing slot cleared after completion",
         !ix.hasCrossingCar(Direction::NORTH));

    // Phase 3: next tick — second vehicle starts crossing (slot is now free)
    ix.update(0.5f);

    TEST("NORTH second vehicle starts crossing on next green tick",
         ix.hasCrossingCar(Direction::NORTH));
    TEST("NORTH queue now empty after second vehicle departs",
         ix.getRoad(Direction::NORTH)->getQueueLane()->isEmpty());

    // Phase 4: despawn — removeVehicle() once vehicle exits screen
    Lane* southArrival = ix.getRoad(Direction::SOUTH)->getArrivalLane();
    TEST("SOUTH arrival has 1 vehicle before despawn", southArrival->getVehicles().size() == 1);
    southArrival->removeVehicle();
    TEST("SOUTH arrival empty after despawn", southArrival->isEmpty());
}

int main() {
    test_car();
    test_lane();
    test_light_controller();
    test_intersection();

    cout << "\n" << passed << " passed, " << failed << " failed\n";
    return failed > 0 ? 1 : 0;
}
