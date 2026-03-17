#include "TrafficLight.h"

TrafficLight:: TrafficLight(int id, LightState lightState, Road* road){
    this->id = id;
    this->lightState = lightState;
    this->timer = 0; // Initialize timer to 0
    this->road = road; // Associate the traffic light with a road
}

int TrafficLight:: getId(){
    return this->id;
}

LightState TrafficLight:: getState(){
    return this->lightState;
}

Road* TrafficLight:: getRoad(){
    return this->road;
}

void TrafficLight::update(int deltaTime){
    // Green -> Yellow -> Read -> Green:
    this->timer += deltaTime;

    switch (this->lightState) {
        case LightState::GREEN:
            if (this->timer >= GREEN_DURATION) {
                this->lightState = LightState::YELLOW;
                this->timer = 0; // Reset timer for the next state
            }
            break;
        case LightState::YELLOW:
            if (this->timer >= YELLOW_DURATION) {
                this->lightState = LightState::RED;
                this->timer = 0; // Reset timer for the next state
            }
            break;
        case LightState::RED:
            if (this->timer >= RED_DURATION) {
                this->lightState = LightState::GREEN;
                this->timer = 0; // Reset timer for the next state
            }
            break;
        default:
            break;
    }
}