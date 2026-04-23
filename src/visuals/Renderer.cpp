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

// ── Public entry point ───────────────────────────────────
void Renderer::draw() {
    drawRoads();
    drawLaneDivisions();
    drawStopLines();
    drawTrafficLights();
    drawCars();
}

// ── Roads ────────────────────────────────────────────────
void Renderer::drawRoads() {
    sf::Color roadColor(75, 75, 75);

    sf::RectangleShape northRoad(sf::Vector2f(Config::ROAD_WIDTH, Config::INTERSECTION_Y));
    northRoad.setFillColor(roadColor);
    northRoad.setPosition(sf::Vector2f(Config::INTERSECTION_X, 0));
    window.draw(northRoad);

    sf::RectangleShape southRoad(sf::Vector2f(Config::ROAD_WIDTH,
        Config::WINDOW_H - Config::INTERSECTION_Y - Config::INTERSECTION_SIZE));
    southRoad.setFillColor(roadColor);
    southRoad.setPosition(sf::Vector2f(Config::INTERSECTION_X,
        Config::INTERSECTION_Y + Config::INTERSECTION_SIZE));
    window.draw(southRoad);

    sf::RectangleShape westRoad(sf::Vector2f(Config::INTERSECTION_X, Config::ROAD_WIDTH));
    westRoad.setFillColor(roadColor);
    westRoad.setPosition(sf::Vector2f(0, Config::INTERSECTION_Y));
    window.draw(westRoad);

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

    // Crosswalk stripes at each edge of the intersection box
    sf::Color cwColor(210, 210, 210);
    const int stripeW = 8, stripeGap = 6, stripeLen = 18;

    // North edge crosswalk (horizontal stripes)
    for (int i = 0; i < 8; ++i) {
        sf::RectangleShape stripe(sf::Vector2f(stripeLen, stripeW));
        stripe.setFillColor(cwColor);
        stripe.setPosition(sf::Vector2f(
            Config::INTERSECTION_X + 10 + i * (stripeLen + stripeGap),
            Config::INTERSECTION_Y - stripeW - 2));
        window.draw(stripe);
    }
    // South edge crosswalk
    for (int i = 0; i < 8; ++i) {
        sf::RectangleShape stripe(sf::Vector2f(stripeLen, stripeW));
        stripe.setFillColor(cwColor);
        stripe.setPosition(sf::Vector2f(
            Config::INTERSECTION_X + 10 + i * (stripeLen + stripeGap),
            Config::INTERSECTION_Y + Config::INTERSECTION_SIZE + 2));
        window.draw(stripe);
    }
    // West edge crosswalk (vertical stripes)
    for (int i = 0; i < 8; ++i) {
        sf::RectangleShape stripe(sf::Vector2f(stripeW, stripeLen));
        stripe.setFillColor(cwColor);
        stripe.setPosition(sf::Vector2f(
            Config::INTERSECTION_X - stripeW - 2,
            Config::INTERSECTION_Y + 10 + i * (stripeLen + stripeGap)));
        window.draw(stripe);
    }
    // East edge crosswalk
    for (int i = 0; i < 8; ++i) {
        sf::RectangleShape stripe(sf::Vector2f(stripeW, stripeLen));
        stripe.setFillColor(cwColor);
        stripe.setPosition(sf::Vector2f(
            Config::INTERSECTION_X + Config::INTERSECTION_SIZE + 2,
            Config::INTERSECTION_Y + 10 + i * (stripeLen + stripeGap)));
        window.draw(stripe);
    }
}

// ── Lane dividers (dashed center lines) ─────────────────
void Renderer::drawLaneDivisions() {
    sf::Color divColor(255, 220, 0);
    const float dashLen = 20.f, dashGap = 15.f, thickness = 3.f;

    // Vertical center line — N/S road
    float vx = Config::INTERSECTION_X + Config::LANE_WIDTH;
    for (float y = 0; y < Config::WINDOW_H; y += dashLen + dashGap) {
        // Skip through the intersection box
        if (y + dashLen > Config::INTERSECTION_Y && y < Config::INTERSECTION_Y + Config::INTERSECTION_SIZE)
            continue;
        sf::RectangleShape dash(sf::Vector2f(thickness, dashLen));
        dash.setFillColor(divColor);
        dash.setPosition(sf::Vector2f(vx, y));
        window.draw(dash);
    }

    // Horizontal center line — E/W road
    float hy = Config::INTERSECTION_Y + Config::LANE_WIDTH;
    for (float x = 0; x < Config::WINDOW_W; x += dashLen + dashGap) {
        if (x + dashLen > Config::INTERSECTION_X && x < Config::INTERSECTION_X + Config::INTERSECTION_SIZE)
            continue;
        sf::RectangleShape dash(sf::Vector2f(dashLen, thickness));
        dash.setFillColor(divColor);
        dash.setPosition(sf::Vector2f(x, hy));
        window.draw(dash);
    }
}

// ── Stop lines ───────────────────────────────────────────
void Renderer::drawStopLines() {
    sf::Color stopColor(255, 255, 255);
    const float thickness = 4.f;

    // North approach — horizontal line just above intersection
    sf::RectangleShape northStop(sf::Vector2f(Config::LANE_WIDTH, thickness));
    northStop.setFillColor(stopColor);
    northStop.setPosition(sf::Vector2f(Config::INTERSECTION_X, Config::INTERSECTION_Y - thickness));
    window.draw(northStop);

    // South approach — just below intersection, on the right lane
    sf::RectangleShape southStop(sf::Vector2f(Config::LANE_WIDTH, thickness));
    southStop.setFillColor(stopColor);
    southStop.setPosition(sf::Vector2f(
        Config::INTERSECTION_X + Config::LANE_WIDTH,
        Config::INTERSECTION_Y + Config::INTERSECTION_SIZE));
    window.draw(southStop);

    // West approach — vertical line just left of intersection
    sf::RectangleShape westStop(sf::Vector2f(thickness, Config::LANE_WIDTH));
    westStop.setFillColor(stopColor);
    westStop.setPosition(sf::Vector2f(Config::INTERSECTION_X - thickness, Config::INTERSECTION_Y + Config::LANE_WIDTH));
    window.draw(westStop);

    // East approach — just right of intersection, on the top lane
    sf::RectangleShape eastStop(sf::Vector2f(thickness, Config::LANE_WIDTH));
    eastStop.setFillColor(stopColor);
    eastStop.setPosition(sf::Vector2f(
        Config::INTERSECTION_X + Config::INTERSECTION_SIZE,
        Config::INTERSECTION_Y));
    window.draw(eastStop);
}

// ── Three-signal traffic light housing ──────────────────
void Renderer::drawThreeSignalLight(float x, float y, LightState state) {
    const float housing_w = 24.f;
    const float housing_h = 66.f;
    const float light_r   =  8.f;
    const float padding   =  5.f;

    // Pole
    sf::RectangleShape pole(sf::Vector2f(4.f, 20.f));
    pole.setFillColor(sf::Color(50, 50, 50));
    pole.setPosition(sf::Vector2f(x + housing_w / 2.f - 2.f, y + housing_h));
    window.draw(pole);

    // Housing box
    sf::RectangleShape housing(sf::Vector2f(housing_w, housing_h));
    housing.setFillColor(sf::Color(25, 25, 25));
    housing.setOutlineColor(sf::Color(10, 10, 10));
    housing.setOutlineThickness(2.f);
    housing.setPosition(sf::Vector2f(x, y));
    window.draw(housing);

    float cx = x + housing_w / 2.f - light_r;
    float slotH = (housing_h - 2.f * padding) / 3.f;

    // Red (top)
    sf::CircleShape redCirc(light_r);
    bool redOn = (state == LightState::RED);
    redCirc.setFillColor(redOn ? sf::Color(255, 40, 40) : sf::Color(70, 10, 10));
    redCirc.setPosition(sf::Vector2f(cx, y + padding + 0 * slotH + (slotH - 2.f * light_r) / 2.f));
    window.draw(redCirc);

    // Yellow (middle)
    sf::CircleShape yelCirc(light_r);
    bool yelOn = (state == LightState::YELLOW);
    yelCirc.setFillColor(yelOn ? sf::Color(255, 210, 0) : sf::Color(70, 55, 0));
    yelCirc.setPosition(sf::Vector2f(cx, y + padding + 1.f * slotH + (slotH - 2.f * light_r) / 2.f));
    window.draw(yelCirc);

    // Green (bottom)
    sf::CircleShape grnCirc(light_r);
    bool grnOn = (state == LightState::GREEN);
    grnCirc.setFillColor(grnOn ? sf::Color(0, 230, 0) : sf::Color(0, 60, 0));
    grnCirc.setPosition(sf::Vector2f(cx, y + padding + 2.f * slotH + (slotH - 2.f * light_r) / 2.f));
    window.draw(grnCirc);
}

// ── Traffic lights — one at each corner ─────────────────
void Renderer::drawTrafficLights() {
    LightController* lc = intersection.getLightController();

    const float hw      = 24.f;   // housing width
    const float hh      = 66.f;   // housing height (matches drawThreeSignalLight)
    const float margin  = 12.f;

    float ix = Config::INTERSECTION_X;
    float iy = Config::INTERSECTION_Y;
    float is = Config::INTERSECTION_SIZE;

    // NW corner — North signal (cars approaching from north, traveling south)
    drawThreeSignalLight(ix - hw - margin, iy - hh - margin,
                         lc->getState(Direction::NORTH));

    // NE corner — East signal (cars approaching from east, traveling west)
    drawThreeSignalLight(ix + is + margin, iy - hh - margin,
                         lc->getState(Direction::EAST));

    // SE corner — South signal (cars approaching from south, traveling north)
    drawThreeSignalLight(ix + is + margin, iy + is + margin,
                         lc->getState(Direction::SOUTH));

    // SW corner — West signal (cars approaching from west, traveling east)
    drawThreeSignalLight(ix - hw - margin, iy + is + margin,
                         lc->getState(Direction::WEST));
}

// ── Color helper ─────────────────────────────────────────
sf::Color Renderer::parseCarColor(const std::string& colorStr) {
    if (colorStr == "red")    return sf::Color(210,  55,  55);
    if (colorStr == "blue")   return sf::Color( 55, 100, 215);
    if (colorStr == "green")  return sf::Color( 50, 170,  70);
    if (colorStr == "yellow") return sf::Color(220, 200,  50);
    if (colorStr == "white")  return sf::Color(230, 230, 230);
    if (colorStr == "black")  return sf::Color( 35,  35,  35);
    if (colorStr == "orange") return sf::Color(220, 110,  40);
    if (colorStr == "purple") return sf::Color(140,  55, 185);
    if (colorStr == "cyan")   return sf::Color( 40, 190, 210);
    return sf::Color(180, 80, 80);
}

// ── Detailed car drawing ─────────────────────────────────
void Renderer::drawSingleCar(float x, float y, Direction dir, sf::Color body) {
    bool vertical = (dir == Direction::NORTH || dir == Direction::SOUTH);

    // Portrait for N/S, landscape for E/W
    float bw = vertical ? Config::CAR_W : Config::CAR_H;
    float bh = vertical ? Config::CAR_H : Config::CAR_W;

    // Darker shade for outline/roof panel
    sf::Color dark(
        static_cast<uint8_t>(body.r * 0.6f),
        static_cast<uint8_t>(body.g * 0.6f),
        static_cast<uint8_t>(body.b * 0.6f));

    // Body
    sf::RectangleShape bodyShape(sf::Vector2f(bw, bh));
    bodyShape.setFillColor(body);
    bodyShape.setOutlineColor(dark);
    bodyShape.setOutlineThickness(1.5f);
    bodyShape.setPosition(sf::Vector2f(x, y));
    window.draw(bodyShape);

    // Roof panel (darker center strip)
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

    sf::Color windshield(150, 200, 235, 220);
    sf::Color windshieldRear(110, 155, 180, 180);
    sf::Color headlightCol(255, 245, 160);
    sf::Color taillightCol(200,  30,  30);
    sf::Color wheelCol(28, 28, 28);

    if (vertical) {
        // Front is where car is heading INTO the intersection:
        //   NORTH cars travel south → front at bottom
        //   SOUTH cars travel north → front at top
        bool frontBottom = (dir == Direction::NORTH);

        float frontY = frontBottom ? y + bh - 14.f : y;
        float rearY  = frontBottom ? y              : y + bh - 14.f;

        // Front windshield
        sf::RectangleShape ws(sf::Vector2f(bw - 8.f, 10.f));
        ws.setFillColor(windshield);
        ws.setPosition(sf::Vector2f(x + 4.f, frontY + 2.f));
        window.draw(ws);

        // Rear windshield
        sf::RectangleShape rws(sf::Vector2f(bw - 10.f, 8.f));
        rws.setFillColor(windshieldRear);
        rws.setPosition(sf::Vector2f(x + 5.f, rearY + 3.f));
        window.draw(rws);

        // Headlights
        float headY = frontBottom ? y + bh - 5.f : y;
        for (float hx : {x + 3.f, x + bw - 9.f}) {
            sf::RectangleShape hl(sf::Vector2f(6.f, 4.f));
            hl.setFillColor(headlightCol);
            hl.setPosition(sf::Vector2f(hx, headY));
            window.draw(hl);
        }

        // Taillights
        float tailY = frontBottom ? y : y + bh - 5.f;
        for (float tx : {x + 3.f, x + bw - 9.f}) {
            sf::RectangleShape tl(sf::Vector2f(6.f, 4.f));
            tl.setFillColor(taillightCol);
            tl.setPosition(sf::Vector2f(tx, tailY));
            window.draw(tl);
        }

        // Wheels (stick out on sides)
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
        // EAST cars travel west → front at left
        // WEST cars travel east → front at right
        bool frontLeft = (dir == Direction::EAST);

        float frontX = frontLeft ? x              : x + bw - 14.f;
        float rearX  = frontLeft ? x + bw - 14.f : x;

        // Front windshield
        sf::RectangleShape ws(sf::Vector2f(10.f, bh - 8.f));
        ws.setFillColor(windshield);
        ws.setPosition(sf::Vector2f(frontX + 2.f, y + 4.f));
        window.draw(ws);

        // Rear windshield
        sf::RectangleShape rws(sf::Vector2f(8.f, bh - 10.f));
        rws.setFillColor(windshieldRear);
        rws.setPosition(sf::Vector2f(rearX + 3.f, y + 5.f));
        window.draw(rws);

        // Headlights
        float headX = frontLeft ? x : x + bw - 5.f;
        for (float hy : {y + 3.f, y + bh - 9.f}) {
            sf::RectangleShape hl(sf::Vector2f(4.f, 6.f));
            hl.setFillColor(headlightCol);
            hl.setPosition(sf::Vector2f(headX, hy));
            window.draw(hl);
        }

        // Taillights
        float tailX = frontLeft ? x + bw - 5.f : x;
        for (float ty : {y + 3.f, y + bh - 9.f}) {
            sf::RectangleShape tl(sf::Vector2f(4.f, 6.f));
            tl.setFillColor(taillightCol);
            tl.setPosition(sf::Vector2f(tailX, ty));
            window.draw(tl);
        }

        // Wheels (stick out on top/bottom)
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

// ── Direction helper ─────────────────────────────────────
Direction Renderer::opposite(Direction d) {
    switch (d) {
        case Direction::NORTH: return Direction::SOUTH;
        case Direction::SOUTH: return Direction::NORTH;
        case Direction::EAST:  return Direction::WEST;
        case Direction::WEST:  return Direction::EAST;
    }
    return d;
}

// ── Car position animation ───────────────────────────────
// Moves each car smoothly toward its computed queue/arrival target.
// Cars with no position set are placed at the off-screen spawn first.
void Renderer::updateCarPositions(float dt) {
    const float gap    = 6.f;
    const float margin = 5.f;
    // Horizontal car footprint in x (bw = CAR_H for E/W cars)
    const float hLen   = Config::CAR_H;

    for (auto dir : {Direction::NORTH, Direction::SOUTH,
                     Direction::EAST,  Direction::WEST}) {
        Road* road = intersection.getRoad(dir);

        // ---- queue lane ----
        auto& qcars = road->getQueueLane()->getCars();
        for (int i = 0; i < (int)qcars.size(); ++i) {
            float tx, ty;
            // car[0] stops just before the stop line; car[i] stacks behind it
            if (dir == Direction::NORTH) {
                tx = Config::NORTH_SPAWN_X;
                ty = Config::INTERSECTION_Y - Config::CAR_H - margin - i * (Config::CAR_H + gap);
            } else if (dir == Direction::SOUTH) {
                tx = Config::SOUTH_SPAWN_X;
                ty = Config::INTERSECTION_Y + Config::INTERSECTION_SIZE + margin + i * (Config::CAR_H + gap);
            } else if (dir == Direction::EAST) {
                tx = Config::INTERSECTION_X + Config::INTERSECTION_SIZE + margin + i * (hLen + gap);
                ty = Config::EAST_SPAWN_Y;
            } else {
                tx = Config::INTERSECTION_X - hLen - margin - i * (hLen + gap);
                ty = Config::WEST_SPAWN_Y;
            }

            if (!qcars[i].isPosSet()) {
                // Place new car off-screen in the correct approach direction
                float sx, sy;
                if      (dir == Direction::NORTH) { sx = tx; sy = -(Config::CAR_H + 10.f); }
                else if (dir == Direction::SOUTH) { sx = tx; sy = Config::WINDOW_H + 10.f; }
                else if (dir == Direction::EAST)  { sx = Config::WINDOW_W + 10.f; sy = ty; }
                else                              { sx = -(hLen + 10.f);           sy = ty; }
                qcars[i].setPos(sx, sy);
            }

            float px = qcars[i].getPx(), py = qcars[i].getPy();
            float dx = tx - px, dy = ty - py;
            float dist = std::sqrt(dx * dx + dy * dy);
            if (dist > 1.f) {
                float step = std::min(Config::CAR_SPEED * dt, dist);
                qcars[i].setPos(px + dx / dist * step, py + dy / dist * step);
            }
        }

        // ---- crossing car (in transit through intersection box) ----
        if (intersection.hasCrossingCar(dir)) {
            Car& cc = intersection.getCrossingCar(dir);

            // tx/ty = exit point (far edge of intersection, car fully clear)
            // sx/sy = entry point (near edge, where the car starts crossing)
            float tx, ty, sx, sy;
            if (dir == Direction::NORTH) {
                tx = Config::NORTH_SPAWN_X;
                ty = (float)(Config::INTERSECTION_Y + Config::INTERSECTION_SIZE);
                sx = tx; sy = (float)(Config::INTERSECTION_Y - Config::CAR_H);
            } else if (dir == Direction::SOUTH) {
                tx = Config::SOUTH_SPAWN_X;
                ty = (float)(Config::INTERSECTION_Y - Config::CAR_H);
                sx = tx; sy = (float)(Config::INTERSECTION_Y + Config::INTERSECTION_SIZE);
            } else if (dir == Direction::EAST) {
                tx = (float)(Config::INTERSECTION_X - Config::CAR_H);
                ty = Config::EAST_SPAWN_Y;
                sx = (float)(Config::INTERSECTION_X + Config::INTERSECTION_SIZE); sy = ty;
            } else {
                tx = (float)(Config::INTERSECTION_X + Config::INTERSECTION_SIZE);
                ty = Config::WEST_SPAWN_Y;
                sx = (float)(Config::INTERSECTION_X - Config::CAR_H); sy = ty;
            }

            if (!cc.isPosSet()) cc.setPos(sx, sy);

            float px = cc.getPx(), py = cc.getPy();
            float dx = tx - px, dy = ty - py;
            float dist = std::sqrt(dx * dx + dy * dy);
            if (dist > 1.f) {
                float step = std::min(Config::CAR_SPEED * dt, dist);
                cc.setPos(px + dx / dist * step, py + dy / dist * step);
            } else {
                cc.setPos(tx, ty);
                intersection.completeCrossing(dir); // animation done → commit to arrival
            }
        }

        // ---- arrival lane: cars continue moving and despawn when off-screen ----
        Lane* arrLane = road->getArrivalLane();
        auto& acars = arrLane->getCars();
        float step = Config::CAR_SPEED * dt;

        for (auto& car : acars) {
            if (!car.isPosSet()) {
                // Safety fallback — normally position is inherited from crossing
                if      (dir == Direction::NORTH) car.setPos(Config::NORTH_ARRIVAL_X, (float)Config::INTERSECTION_Y);
                else if (dir == Direction::SOUTH) car.setPos(Config::SOUTH_ARRIVAL_X, (float)(Config::INTERSECTION_Y + Config::INTERSECTION_SIZE));
                else if (dir == Direction::EAST)  car.setPos((float)(Config::INTERSECTION_X + Config::INTERSECTION_SIZE), Config::EAST_ARRIVAL_Y);
                else                              car.setPos((float)Config::INTERSECTION_X, Config::WEST_ARRIVAL_Y);
            }
            // Move in the direction the car is travelling (away from intersection)
            float px = car.getPx(), py = car.getPy();
            if      (dir == Direction::NORTH) car.setPos(px, py - step);
            else if (dir == Direction::SOUTH) car.setPos(px, py + step);
            else if (dir == Direction::EAST)  car.setPos(px + step, py);
            else                              car.setPos(px - step, py);
        }

        // Despawn from the front once fully off-screen
        while (!arrLane->isEmpty()) {
            const Car& front = arrLane->getCars().front();
            bool off =
                (dir == Direction::NORTH && front.getPy() + Config::CAR_H < 0.f)         ||
                (dir == Direction::SOUTH && front.getPy() > (float)Config::WINDOW_H)      ||
                (dir == Direction::EAST  && front.getPx() > (float)Config::WINDOW_W)      ||
                (dir == Direction::WEST  && front.getPx() + Config::CAR_H < 0.f);
            if (off) arrLane->removeCar();
            else break;
        }
    }
}

// ── Cars ─────────────────────────────────────────────────
// Renders queue and arrival lane cars using their animated positions.
void Renderer::drawCars() {
    for (auto dir : {Direction::NORTH, Direction::SOUTH,
                     Direction::EAST,  Direction::WEST}) {
        Road* road = intersection.getRoad(dir);

        const auto& qcars = road->getQueueLane()->getCars();
        for (const auto& car : qcars) {
            if (!car.isPosSet()) continue;
            drawSingleCar(car.getPx(), car.getPy(), dir, parseCarColor(car.getColor()));
        }

        // Crossing car travels straight through the intersection box
        if (intersection.hasCrossingCar(dir)) {
            Car& cc = intersection.getCrossingCar(dir);
            if (cc.isPosSet())
                drawSingleCar(cc.getPx(), cc.getPy(), dir, parseCarColor(cc.getColor()));
        }

        // Arrival lane cars face the opposite direction (they've crossed through)
        const auto& acars = road->getArrivalLane()->getCars();
        Direction exitDir = opposite(dir);
        for (const auto& car : acars) {
            if (!car.isPosSet()) continue;
            drawSingleCar(car.getPx(), car.getPy(), exitDir, parseCarColor(car.getColor()));
        }
    }
}
