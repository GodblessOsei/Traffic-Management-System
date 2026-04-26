#include "Renderer.h"
#include "Config.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <map>
#include <string>
#include <vector>

Renderer::Renderer(sf::RenderWindow& window, Intersection& intersection)
    : window(window), intersection(intersection)
{}

// Public entry point 
void Renderer::draw() {
    // Passes are ordered back-to-front:
    // roads first so everything else sits on top of the road.
    drawRoads();
    drawLaneDivisions();
    drawStopLines();
    drawTrafficLights();
    drawVehicles();
}

// Roads
void Renderer::drawRoads() {
    sf::Color roadColor(75, 75, 75);

    // North road: spans from y=0 to the top edge of the intersection box.
    // Width = ROAD_WIDTH (both lanes). Height = INTERSECTION_Y (distance from
    // the window top to the intersection box top).
    sf::RectangleShape northRoad(sf::Vector2f(Config::ROAD_WIDTH, Config::INTERSECTION_Y));
    northRoad.setFillColor(roadColor);
    northRoad.setPosition(sf::Vector2f(Config::INTERSECTION_X, 0));
    window.draw(northRoad);

    // South road: starts at the bottom edge of the intersection box and runs
    // to the bottom of the window. Height = WINDOW_H - (box top + box size).
    sf::RectangleShape southRoad(sf::Vector2f(Config::ROAD_WIDTH,
        Config::WINDOW_H - Config::INTERSECTION_Y - Config::INTERSECTION_SIZE));
    southRoad.setFillColor(roadColor);
    southRoad.setPosition(sf::Vector2f(Config::INTERSECTION_X,
        Config::INTERSECTION_Y + Config::INTERSECTION_SIZE));
    window.draw(southRoad);

    // West road: spans from x=0 to the left edge of the intersection box.
    sf::RectangleShape westRoad(sf::Vector2f(Config::INTERSECTION_X, Config::ROAD_WIDTH));
    westRoad.setFillColor(roadColor);
    westRoad.setPosition(sf::Vector2f(0, Config::INTERSECTION_Y));
    window.draw(westRoad);

    // East road: starts at the right edge of the intersection box and runs
    // to the right edge of the window.
    sf::RectangleShape eastRoad(sf::Vector2f(
        Config::WINDOW_W - Config::INTERSECTION_X - Config::INTERSECTION_SIZE,
        Config::ROAD_WIDTH));
    eastRoad.setFillColor(roadColor);
    eastRoad.setPosition(sf::Vector2f(Config::INTERSECTION_X + Config::INTERSECTION_SIZE,
        Config::INTERSECTION_Y));
    window.draw(eastRoad);

    // Intersection box: slightly darker than the road so it reads as a separate
    // zone at a glance, even though vehicles pass through it continuously.
    sf::RectangleShape box(sf::Vector2f(Config::INTERSECTION_SIZE, Config::INTERSECTION_SIZE));
    box.setFillColor(sf::Color(60, 60, 60));
    box.setPosition(sf::Vector2f(Config::INTERSECTION_X, Config::INTERSECTION_Y));
    window.draw(box);

    // Crosswalk stripes along all four edges of the intersection box.
    // stripeLen × stripeW are the dimensions of each stripe.
    // stripeGap is the gap between stripes. 8 stripes per edge fits the 200px box.
    sf::Color cwColor(210, 210, 210);
    const int stripeW = 8, stripeGap = 6, stripeLen = 18;

    // North edge — horizontal stripes just above the box (y offset = -stripeW - 2px).
    // The 10px left inset keeps stripes away from the box corners.
    for (int i = 0; i < 8; ++i) {
        sf::RectangleShape stripe(sf::Vector2f(stripeLen, stripeW));
        stripe.setFillColor(cwColor);
        stripe.setPosition(sf::Vector2f(
            Config::INTERSECTION_X + 10 + i * (stripeLen + stripeGap),
            Config::INTERSECTION_Y - stripeW - 2));
        window.draw(stripe);
    }
    // South edge — horizontal stripes just below the box.
    for (int i = 0; i < 8; ++i) {
        sf::RectangleShape stripe(sf::Vector2f(stripeLen, stripeW));
        stripe.setFillColor(cwColor);
        stripe.setPosition(sf::Vector2f(
            Config::INTERSECTION_X + 10 + i * (stripeLen + stripeGap),
            Config::INTERSECTION_Y + Config::INTERSECTION_SIZE + 2));
        window.draw(stripe);
    }
    // West edge — vertical stripes (swapped dimensions: stripeW wide, stripeLen tall)
    // drawn just to the left of the box.
    for (int i = 0; i < 8; ++i) {
        sf::RectangleShape stripe(sf::Vector2f(stripeW, stripeLen));
        stripe.setFillColor(cwColor);
        stripe.setPosition(sf::Vector2f(
            Config::INTERSECTION_X - stripeW - 2,
            Config::INTERSECTION_Y + 10 + i * (stripeLen + stripeGap)));
        window.draw(stripe);
    }
    // East edge — vertical stripes just to the right of the box.
    for (int i = 0; i < 8; ++i) {
        sf::RectangleShape stripe(sf::Vector2f(stripeW, stripeLen));
        stripe.setFillColor(cwColor);
        stripe.setPosition(sf::Vector2f(
            Config::INTERSECTION_X + Config::INTERSECTION_SIZE + 2,
            Config::INTERSECTION_Y + 10 + i * (stripeLen + stripeGap)));
        window.draw(stripe);
    }
}

// Lane dividers (dashed center lines) 
void Renderer::drawLaneDivisions() {
    sf::Color divColor(255, 220, 0);
    const float dashLen = 20.f, dashGap = 15.f, thickness = 3.f;

    // Vertical divider sits at x = INTERSECTION_X + LANE_WIDTH, which is the
    // boundary between the two N/S lanes. We skip y values that fall inside the
    // intersection box — there are no lane markings inside the box itself.
    float vx = Config::INTERSECTION_X + Config::LANE_WIDTH;
    for (float y = 0; y < Config::WINDOW_H; y += dashLen + dashGap) {
        if (y + dashLen > Config::INTERSECTION_Y && y < Config::INTERSECTION_Y + Config::INTERSECTION_SIZE)
            continue; // skip the intersection box region
        sf::RectangleShape dash(sf::Vector2f(thickness, dashLen));
        dash.setFillColor(divColor);
        dash.setPosition(sf::Vector2f(vx, y));
        window.draw(dash);
    }

    // Horizontal divider sits at y = INTERSECTION_Y + LANE_WIDTH, the boundary
    // between the two E/W lanes. Similarly skips the intersection box.
    float hy = Config::INTERSECTION_Y + Config::LANE_WIDTH;
    for (float x = 0; x < Config::WINDOW_W; x += dashLen + dashGap) {
        if (x + dashLen > Config::INTERSECTION_X && x < Config::INTERSECTION_X + Config::INTERSECTION_SIZE)
            continue; // skip the intersection box region
        sf::RectangleShape dash(sf::Vector2f(dashLen, thickness));
        dash.setFillColor(divColor);
        dash.setPosition(sf::Vector2f(x, hy));
        window.draw(dash);
    }
}

//  Stop lines 
void Renderer::drawStopLines() {
    sf::Color stopColor(255, 255, 255);
    const float thickness = 4.f;

    // North stop line: flush against the top edge of the box (y = INTERSECTION_Y - 4).
    // Width = LANE_WIDTH — covers only the left (southbound queue) lane.
    sf::RectangleShape northStop(sf::Vector2f(Config::LANE_WIDTH, thickness));
    northStop.setFillColor(stopColor);
    northStop.setPosition(sf::Vector2f(Config::INTERSECTION_X, Config::INTERSECTION_Y - thickness));
    window.draw(northStop);

    // South stop line: flush against the bottom edge of the box.
    // Offset right by LANE_WIDTH so it covers the right (northbound queue) lane.
    sf::RectangleShape southStop(sf::Vector2f(Config::LANE_WIDTH, thickness));
    southStop.setFillColor(stopColor);
    southStop.setPosition(sf::Vector2f(
        Config::INTERSECTION_X + Config::LANE_WIDTH,
        Config::INTERSECTION_Y + Config::INTERSECTION_SIZE));
    window.draw(southStop);

    // West stop line: flush against the left edge of the box.
    // Offset down by LANE_WIDTH so it covers the bottom (eastbound queue) lane.
    sf::RectangleShape westStop(sf::Vector2f(thickness, Config::LANE_WIDTH));
    westStop.setFillColor(stopColor);
    westStop.setPosition(sf::Vector2f(Config::INTERSECTION_X - thickness, Config::INTERSECTION_Y + Config::LANE_WIDTH));
    window.draw(westStop);

    // East stop line: flush against the right edge of the box.
    // Starts at INTERSECTION_Y (top of road) to cover the top (westbound queue) lane.
    sf::RectangleShape eastStop(sf::Vector2f(thickness, Config::LANE_WIDTH));
    eastStop.setFillColor(stopColor);
    eastStop.setPosition(sf::Vector2f(
        Config::INTERSECTION_X + Config::INTERSECTION_SIZE,
        Config::INTERSECTION_Y));
    window.draw(eastStop);
}

// ── Three-signal traffic light housing ──────────────────
void Renderer::drawThreeSignalLight(float x, float y, LightState state) {
    const float housing_w = 24.f, housing_h = 66.f, light_r = 8.f, padding = 5.f;

    // Pole: a narrow dark rectangle extending below the housing.
    sf::RectangleShape pole(sf::Vector2f(4.f, 20.f));
    pole.setFillColor(sf::Color(50, 50, 50));
    pole.setPosition(sf::Vector2f(x + housing_w / 2.f - 2.f, y + housing_h));
    window.draw(pole);

    // Housing: dark box drawn on top of the pole so its base covers the pole top.
    sf::RectangleShape housing(sf::Vector2f(housing_w, housing_h));
    housing.setFillColor(sf::Color(25, 25, 25));
    housing.setOutlineColor(sf::Color(10, 10, 10));
    housing.setOutlineThickness(2.f);
    housing.setPosition(sf::Vector2f(x, y));
    window.draw(housing);

    // Divide the housing interior (minus top/bottom padding) into three equal slots
    // so each bulb gets a vertical third of the usable height.
    float cx = x + housing_w / 2.f - light_r;
    float slotH = (housing_h - 2.f * padding) / 3.f;

    // Each bulb is centred vertically within its slot:
    //   top of slot + (slot height - bulb diameter) / 2
    // Only the active bulb is fully lit; inactive bulbs render as a dim version
    // of their colour so the housing shows all three positions at all times.

    sf::CircleShape redCirc(light_r);
    redCirc.setFillColor((state == LightState::RED) ? sf::Color(255, 40, 40) : sf::Color(70, 10, 10));
    redCirc.setPosition(sf::Vector2f(cx, y + padding + 0 * slotH + (slotH - 2.f * light_r) / 2.f));
    window.draw(redCirc);

    sf::CircleShape yelCirc(light_r);
    yelCirc.setFillColor((state == LightState::YELLOW) ? sf::Color(255, 210, 0) : sf::Color(70, 55, 0));
    yelCirc.setPosition(sf::Vector2f(cx, y + padding + 1.f * slotH + (slotH - 2.f * light_r) / 2.f));
    window.draw(yelCirc);

    sf::CircleShape grnCirc(light_r);
    grnCirc.setFillColor((state == LightState::GREEN) ? sf::Color(0, 230, 0) : sf::Color(0, 60, 0));
    grnCirc.setPosition(sf::Vector2f(cx, y + padding + 2.f * slotH + (slotH - 2.f * light_r) / 2.f));
    window.draw(grnCirc);
}

// ── Traffic lights — one at each corner ─────────────────
void Renderer::drawTrafficLights() {
    LightController* lc = intersection.getLightController();
    const float hw = 24.f, hh = 66.f, margin = 12.f;
    float ix = Config::INTERSECTION_X, iy = Config::INTERSECTION_Y, is = Config::INTERSECTION_SIZE;

    // Place one housing at each corner of the intersection box.
    // hw/hh are used to back-offset so the housing sits outside the box, not inside.
    //   NW corner — north light (controls northbound queue)
    drawThreeSignalLight(ix - hw - margin, iy - hh - margin,  lc->getState(Direction::NORTH));
    //   NE corner — east light
    drawThreeSignalLight(ix + is + margin, iy - hh - margin,  lc->getState(Direction::EAST));
    //   SE corner — south light
    drawThreeSignalLight(ix + is + margin, iy + is + margin,  lc->getState(Direction::SOUTH));
    //   SW corner — west light
    drawThreeSignalLight(ix - hw - margin, iy + is + margin,  lc->getState(Direction::WEST));
}

// ── Color helper ─────────────────────────────────────────
sf::Color Renderer::parseVehicleColor(const std::string& colorStr) {
    // Slightly desaturated values look better on the dark road than pure primaries.
    if (colorStr == "red")    return sf::Color(210,  55,  55);
    if (colorStr == "blue")   return sf::Color( 55, 100, 215);
    if (colorStr == "green")  return sf::Color( 50, 170,  70);
    if (colorStr == "yellow") return sf::Color(220, 200,  50);
    if (colorStr == "white")  return sf::Color(230, 230, 230);
    if (colorStr == "black")  return sf::Color( 35,  35,  35);
    if (colorStr == "orange") return sf::Color(220, 110,  40);
    if (colorStr == "purple") return sf::Color(140,  55, 185);
    if (colorStr == "cyan")   return sf::Color( 40, 190, 210);
    return sf::Color(180, 80, 80); // fallback pink-red for unrecognised names
}

// ── Detailed vehicle drawing ─────────────────────────────
// vw = narrow dimension, vh = long dimension (portrait contract from Vehicle).
// For N/S travel the long axis is vertical, so bw=vw, bh=vh.
// For E/W travel the long axis is horizontal, so we swap: bw=vh, bh=vw.
void Renderer::drawSingleVehicle(float x, float y, Direction dir, sf::Color body, float vw, float vh) {
    bool vertical = (dir == Direction::NORTH || dir == Direction::SOUTH);

    // Swap dimensions for horizontal travel so the long axis always faces forward.
    float bw = vertical ? vw : vh;
    float bh = vertical ? vh : vw;

    // Derive a darker shade of the body colour for the roof, outline, and rear
    // windshield — gives the illusion of depth without a 3-D renderer.
    sf::Color dark(
        static_cast<uint8_t>(body.r * 0.6f),
        static_cast<uint8_t>(body.g * 0.6f),
        static_cast<uint8_t>(body.b * 0.6f));

    // Body — drawn first so all detail layers paint over it.
    sf::RectangleShape bodyShape(sf::Vector2f(bw, bh));
    bodyShape.setFillColor(body);
    bodyShape.setOutlineColor(dark);
    bodyShape.setOutlineThickness(1.5f);
    bodyShape.setPosition(sf::Vector2f(x, y));
    window.draw(bodyShape);

    // Roof panel — centred over the middle third of the body.
    // For a vertical vehicle: inset 4px horizontally, placed at 30% down.
    // For a horizontal vehicle: inset 4px vertically, placed at 30% across.
    if (vertical) {
        sf::RectangleShape roof(sf::Vector2f(bw - 8.f, bh * 0.40f));
        roof.setFillColor(dark);
        roof.setPosition(sf::Vector2f(x + 4.f, y + bh * 0.30f));
        window.draw(roof);
    } else {
        sf::RectangleShape roof(sf::Vector2f(bw * 0.40f, bh - 8.f));
        roof.setFillColor(dark);
        roof.setPosition(sf::Vector2f(x + bw * 0.30f, y + 4.f));
        window.draw(roof);
    }

    sf::Color windshield(150, 200, 235, 220);    // semi-transparent blue-grey
    sf::Color windshieldRear(110, 155, 180, 180); // darker and more opaque for rear
    sf::Color headlightCol(255, 245, 160);        // warm yellow
    sf::Color taillightCol(200,  30,  30);        // deep red
    sf::Color wheelCol(28, 28, 28);

    if (vertical) {
        // NORTH-facing vehicles move downward toward the intersection, so their
        // front is at the bottom (larger y). SOUTH-facing is the opposite.
        bool frontBottom = (dir == Direction::NORTH);
        float frontY = frontBottom ? y + bh - 14.f : y;
        float rearY  = frontBottom ? y              : y + bh - 14.f;

        // Front windshield — 10px tall, inset 4px from each side.
        sf::RectangleShape ws(sf::Vector2f(bw - 8.f, 10.f));
        ws.setFillColor(windshield);
        ws.setPosition(sf::Vector2f(x + 4.f, frontY + 2.f));
        window.draw(ws);

        // Rear windshield — slightly shorter and darker than the front.
        sf::RectangleShape rws(sf::Vector2f(bw - 10.f, 8.f));
        rws.setFillColor(windshieldRear);
        rws.setPosition(sf::Vector2f(x + 5.f, rearY + 3.f));
        window.draw(rws);

        // Headlights — two small rectangles at the front edge, one each side.
        float headY = frontBottom ? y + bh - 5.f : y;
        for (float hx : {x + 3.f, x + bw - 9.f}) {
            sf::RectangleShape hl(sf::Vector2f(6.f, 4.f));
            hl.setFillColor(headlightCol);
            hl.setPosition(sf::Vector2f(hx, headY));
            window.draw(hl);
        }

        // Tail lights — same geometry as headlights but red, at the rear edge.
        float tailY = frontBottom ? y : y + bh - 5.f;
        for (float tx : {x + 3.f, x + bw - 9.f}) {
            sf::RectangleShape tl(sf::Vector2f(6.f, 4.f));
            tl.setFillColor(taillightCol);
            tl.setPosition(sf::Vector2f(tx, tailY));
            window.draw(tl);
        }

        // Wheels — drawn outside the body bounds (x-5 on the left, x+bw on the right)
        // so they protrude from the sides like real wheel arches.
        // Two axle positions: near the front and near the rear.
        for (float wy : {y + 6.f, y + bh - 16.f}) {
            sf::RectangleShape wl(sf::Vector2f(5.f, 10.f));
            wl.setFillColor(wheelCol);
            wl.setPosition(sf::Vector2f(x - 5.f, wy));
            window.draw(wl);
            sf::RectangleShape wr(sf::Vector2f(5.f, 10.f));
            wr.setFillColor(wheelCol);
            wr.setPosition(sf::Vector2f(x + bw, wy));
            window.draw(wr);
        }

    } else {
        // EAST-facing vehicles move left toward the intersection, so their front
        // is on the left side (smaller x). WEST-facing is the opposite.
        bool frontLeft = (dir == Direction::EAST);
        float frontX = frontLeft ? x              : x + bw - 14.f;
        float rearX  = frontLeft ? x + bw - 14.f : x;

        // Front windshield — 10px wide, inset 4px from top and bottom.
        sf::RectangleShape ws(sf::Vector2f(10.f, bh - 8.f));
        ws.setFillColor(windshield);
        ws.setPosition(sf::Vector2f(frontX + 2.f, y + 4.f));
        window.draw(ws);

        // Rear windshield.
        sf::RectangleShape rws(sf::Vector2f(8.f, bh - 10.f));
        rws.setFillColor(windshieldRear);
        rws.setPosition(sf::Vector2f(rearX + 3.f, y + 5.f));
        window.draw(rws);

        // Headlights at the front edge, one at top and one at bottom.
        float headX = frontLeft ? x : x + bw - 5.f;
        for (float hy : {y + 3.f, y + bh - 9.f}) {
            sf::RectangleShape hl(sf::Vector2f(4.f, 6.f));
            hl.setFillColor(headlightCol);
            hl.setPosition(sf::Vector2f(headX, hy));
            window.draw(hl);
        }

        // Tail lights at the rear edge.
        float tailX = frontLeft ? x + bw - 5.f : x;
        for (float ty : {y + 3.f, y + bh - 9.f}) {
            sf::RectangleShape tl(sf::Vector2f(4.f, 6.f));
            tl.setFillColor(taillightCol);
            tl.setPosition(sf::Vector2f(tailX, ty));
            window.draw(tl);
        }

        // Wheels protrude above (y-5) and below (y+bh) the body.
        for (float wx : {x + 6.f, x + bw - 16.f}) {
            sf::RectangleShape wt(sf::Vector2f(10.f, 5.f));
            wt.setFillColor(wheelCol);
            wt.setPosition(sf::Vector2f(wx, y - 5.f));
            window.draw(wt);
            sf::RectangleShape wb(sf::Vector2f(10.f, 5.f));
            wb.setFillColor(wheelCol);
            wb.setPosition(sf::Vector2f(wx, y + bh));
            window.draw(wb);
        }
    }
}

//  Direction helper
Direction Renderer::opposite(Direction d) {
    switch (d) {
        case Direction::NORTH: return Direction::SOUTH;
        case Direction::SOUTH: return Direction::NORTH;
        case Direction::EAST:  return Direction::WEST;
        case Direction::WEST:  return Direction::EAST;
    }
    return d;
}

//  Vehicle position animation 
void Renderer::updateVehiclePositions(float dt) {
    const float gap    = 6.f;  // pixel gap between queued vehicles
    const float margin = 5.f;  // clearance between the stop line and the first vehicle

    for (auto dir : {Direction::NORTH, Direction::SOUTH,
                     Direction::EAST,  Direction::WEST}) {
        Road* road = intersection.getRoad(dir);

        // queue lane 
        // Vehicles stack back from the stop line. Index 0 is at the front
        // (closest to the intersection), index i is offset further back by
        // i * (vLen + gap). vLen is the vehicle's long dimension.
        auto& qvehicles = road->getQueueLane()->getVehicles();
        for (int i = 0; i < (int)qvehicles.size(); ++i) {
            auto& v = qvehicles[i];
            float vLen = v->getHeight(); // long dimension used for spacing

            // Compute the target screen position for slot i.
            float tx, ty;
            if (dir == Direction::NORTH) {
                tx = Config::NORTH_SPAWN_X;
                ty = Config::INTERSECTION_Y - vLen - margin - i * (vLen + gap);
            } else if (dir == Direction::SOUTH) {
                tx = Config::SOUTH_SPAWN_X;
                ty = Config::INTERSECTION_Y + Config::INTERSECTION_SIZE + margin + i * (vLen + gap);
            } else if (dir == Direction::EAST) {
                tx = Config::INTERSECTION_X + Config::INTERSECTION_SIZE + margin + i * (vLen + gap);
                ty = Config::EAST_SPAWN_Y;
            } else {
                tx = Config::INTERSECTION_X - vLen - margin - i * (vLen + gap);
                ty = Config::WEST_SPAWN_Y;
            }

            // First time we see this vehicle: initialise it off-screen so it
            // slides in rather than appearing at the target position instantly.
            if (!v->isPosSet()) {
                float sx, sy;
                if      (dir == Direction::NORTH) { sx = tx; sy = -(vLen + 10.f); }
                else if (dir == Direction::SOUTH) { sx = tx; sy = Config::WINDOW_H + 10.f; }
                else if (dir == Direction::EAST)  { sx = Config::WINDOW_W + 10.f; sy = ty; }
                else                              { sx = -(vLen + 10.f); sy = ty; }
                v->setPos(sx, sy);
            }

            // Lerp toward the target: compute the remaining distance, normalise
            // the direction vector, and advance by at most CAR_SPEED * dt pixels.
            float px = v->getPx(), py = v->getPy();
            float dx = tx - px, dy = ty - py;
            float dist = std::sqrt(dx * dx + dy * dy);
            if (dist > 1.f) {
                float step = std::min(Config::CAR_SPEED * dt, dist);
                v->setPos(px + dx / dist * step, py + dy / dist * step);
            }
        }

        // crossing vehicle 
        // The crossing vehicle animates from the near edge to the far edge of
        // the intersection box. Once it arrives, completeCrossing() hands it
        // off to the arrival lane on the opposite arm.
        if (intersection.hasCrossingCar(dir)) {
            Vehicle& cc = intersection.getCrossingCar(dir);
            float vLen = cc.getHeight();

            // tx/ty = target (far edge of box); sx/sy = start (near edge of box).
            float tx, ty, sx, sy;
            if (dir == Direction::NORTH) {
                tx = Config::NORTH_SPAWN_X; ty = (float)(Config::INTERSECTION_Y + Config::INTERSECTION_SIZE);
                sx = tx; sy = (float)(Config::INTERSECTION_Y - vLen);
            } else if (dir == Direction::SOUTH) {
                tx = Config::SOUTH_SPAWN_X; ty = (float)(Config::INTERSECTION_Y - vLen);
                sx = tx; sy = (float)(Config::INTERSECTION_Y + Config::INTERSECTION_SIZE);
            } else if (dir == Direction::EAST) {
                tx = (float)(Config::INTERSECTION_X - vLen); ty = Config::EAST_SPAWN_Y;
                sx = (float)(Config::INTERSECTION_X + Config::INTERSECTION_SIZE); sy = ty;
            } else {
                tx = (float)(Config::INTERSECTION_X + Config::INTERSECTION_SIZE); ty = Config::WEST_SPAWN_Y;
                sx = (float)(Config::INTERSECTION_X - vLen); sy = ty;
            }

            if (!cc.isPosSet()) cc.setPos(sx, sy);

            float px = cc.getPx(), py = cc.getPy();
            float dx = tx - px, dy = ty - py;
            float dist = std::sqrt(dx * dx + dy * dy);
            if (dist > 1.f) {
                float step = std::min(Config::CAR_SPEED * dt, dist);
                cc.setPos(px + dx / dist * step, py + dy / dist * step);
            } else {
                // Snap to target and transfer the vehicle to the arrival lane.
                // After this call the crossing slot is empty (nullptr) so the
                // intersection can admit the next waiting vehicle on the next tick.
                cc.setPos(tx, ty);
                intersection.completeCrossing(dir);
            }
        }

        // arrival lane 
        // Exiting vehicles move at full speed in their exit direction with no
        // target — they coast off-screen and are then despawned. We do NOT lerp
        // to a target here because there is no queue to manage on the exit side.
        Lane* arrLane = road->getArrivalLane();
        auto& avehicles = arrLane->getVehicles();
        float step = Config::CAR_SPEED * dt;

        for (auto& v : avehicles) {
            // Initialise position at the far edge of the intersection box if
            // this vehicle was just handed off by completeCrossing().
            if (!v->isPosSet()) {
                if      (dir == Direction::NORTH) v->setPos(Config::NORTH_SPAWN_X, (float)Config::INTERSECTION_Y);
                else if (dir == Direction::SOUTH) v->setPos(Config::SOUTH_SPAWN_X, (float)(Config::INTERSECTION_Y + Config::INTERSECTION_SIZE));
                else if (dir == Direction::EAST)  v->setPos((float)(Config::INTERSECTION_X + Config::INTERSECTION_SIZE), Config::EAST_SPAWN_Y);
                else                              v->setPos((float)Config::INTERSECTION_X, Config::WEST_SPAWN_Y);
            }
            float px = v->getPx(), py = v->getPy();
            if      (dir == Direction::NORTH) v->setPos(px, py - step);
            else if (dir == Direction::SOUTH) v->setPos(px, py + step);
            else if (dir == Direction::EAST)  v->setPos(px + step, py);
            else                              v->setPos(px - step, py);
        }

        // Despawn vehicles that have fully left the window. Only the front of
        // the vector needs checking — vehicles exit in FIFO order, so if the
        // front is still on-screen, nothing behind it can be off-screen yet.
        while (!arrLane->isEmpty()) {
            const auto& front = arrLane->getVehicles().front();
            float vLen = front->getHeight();
            bool off =
                (dir == Direction::NORTH && front->getPy() + vLen < 0.f)             ||
                (dir == Direction::SOUTH && front->getPy() > (float)Config::WINDOW_H) ||
                (dir == Direction::EAST  && front->getPx() > (float)Config::WINDOW_W) ||
                (dir == Direction::WEST  && front->getPx() + vLen < 0.f);
            if (off) arrLane->removeVehicle();
            else break;
        }
    }
}

// Vehicle drawing 
void Renderer::drawVehicles() {
    for (auto dir : {Direction::NORTH, Direction::SOUTH,
                     Direction::EAST,  Direction::WEST}) {
        Road* road = intersection.getRoad(dir);

        // Queue lane — vehicles face toward the intersection (their approach direction).
        const auto& qvehicles = road->getQueueLane()->getVehicles();
        for (const auto& v : qvehicles) {
            if (!v->isPosSet()) continue; // skip vehicles not yet placed on screen
            drawSingleVehicle(v->getPx(), v->getPy(), dir,
                          parseVehicleColor(v->getColor()), v->getWidth(), v->getHeight());
        }

        // Crossing vehicle — same direction as the queue until it completes.
        if (intersection.hasCrossingCar(dir)) {
            Vehicle& cc = intersection.getCrossingCar(dir);
            if (cc.isPosSet())
                drawSingleVehicle(cc.getPx(), cc.getPy(), dir,
                              parseVehicleColor(cc.getColor()), cc.getWidth(), cc.getHeight());
        }

        // Arrival lane — vehicles have exited the box and now travel in the
        // opposite direction, so we pass opposite(dir) to orient them correctly.
        const auto& avehicles = road->getArrivalLane()->getVehicles();
        Direction exitDir = opposite(dir);
        for (const auto& v : avehicles) {
            if (!v->isPosSet()) continue;
            drawSingleVehicle(v->getPx(), v->getPy(), exitDir,
                          parseVehicleColor(v->getColor()), v->getWidth(), v->getHeight());
        }
    }
}
