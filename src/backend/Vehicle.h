#pragma once
#include <string>
#include <memory>

// Abstract base class for all vehicles in the simulation.
class Vehicle
{
protected:
    // static id counter
    static int nextId;

    int   id;                    
    float px = 0.f, py = 0.f;   // Screen-space position, updated by the renderer each frame.
    bool  posSet = false;        // Guards against drawing before the first setPos call.

public:
    // Set Intersection — true when the light is green for this vehicle's lane.
    bool canMove = false;

    Vehicle();
    virtual ~Vehicle() = default;

    // Position information for all vehicles 
    int   getId()    const;
    void  setPos(float x, float y); // Also sets posSet = true on first call.
    float getPx()    const;
    float getPy()    const;
    bool  isPosSet() const; // False until setPos is called

    // Pure virtual interface: each subclass overrides this
    virtual float       getSpeed()  const = 0; // Pixels per second for animation.
    virtual std::string getColor()  const = 0; // resolved RGB color by Renderer::parseVehicleColor.
    virtual std::string getType()   const = 0; // "Car", "Bus", "Bike".

    // Getters for size in portrait orientation of vehicles
    // The renderer swaps width and  height for east/west-travelling vehicles
    virtual float getWidth()  const = 0; // Narrow side 
    virtual float getHeight() const = 0; // Long side  
};
