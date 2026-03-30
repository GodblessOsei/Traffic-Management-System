#include "TrafficLight.h"

// Initialize traffic light identity and target road.
TrafficLight:: TrafficLight(int id, LightState lightState, Road* road){
    this->id = id;
    this->lightState = lightState;
    this->road = road; // Associate the traffic light with a road
}

// Return light identifier.
int TrafficLight:: getId(){
    return this->id;
}

// Return current signal state.
LightState TrafficLight:: getState(){
    return this->lightState;
}

// Return the attached road.
Road* TrafficLight:: getRoad(){
    return this->road;
}

// Update signal state (controller-only path).
void TrafficLight:: setState(LightState newState){
    this->lightState = newState;
}