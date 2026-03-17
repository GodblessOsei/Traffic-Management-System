#include "TrafficLight.h"

TrafficLight:: TrafficLight(int id, LightState lightState, Road* road){
    this->id = id;
    this->lightState = lightState;
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

void TrafficLight:: setState(LightState newState){
    this->lightState = newState;
}