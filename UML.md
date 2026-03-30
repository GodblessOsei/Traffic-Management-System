# Traffic Management System — UML Class Diagram

```mermaid
classDiagram
    direction TB

    %% Enums
    class Direction {
        <<enumeration>>
        NORTH
        SOUTH
        EAST
        WEST
    }

    class LightState {
        <<enumeration>>
        RED
        GREEN
        YELLOW
    }

    class Phase {
        <<enumeration>>
        NS_GREEN
        NS_YELLOW
        EW_GREEN
        EW_YELLOW
    }

    %% Classes
    class Car {
        -int id
        -int speed
        -string colour
        +Car(int id, int speed, string colour)
        +getId() int
        +getSpeed() int
        +getColor() string
    }

    class Lane {
        -int id
        -bool QueueLane
        -queue~Car~ cars
        -Road* road
        +Lane(int id, Road* road, bool QueueLane)
        +addCar(Car car) void
        +removeCar() Car
        +isEmpty() bool
        +getId() int
        +isQueueLane() bool
    }

    class Road {
        -int id
        -Direction direction
        -Lane* queueLane
        -Lane* arrivalLane
        +Road(int id, Direction direction, Lane* queueLane, Lane* arrivalLane)
        +getQueueLane() Lane*
        +getArrivalLane() Lane*
        +getDirection() Direction
        +getId() int
    }

    class TrafficLight {
        -int id
        -LightState lightState
        -Road* road
        +TrafficLight(int id, LightState lightState, Road* road)
        +getId() int
        +getState() LightState
        +getRoad() Road*
        -setState(LightState newState) void
    }

    class LightController {
        -map~Direction, TrafficLight*~ trafficLights
        -Phase currentPhase
        -int timer
        +LightController(map~Direction, TrafficLight*~ trafficLights)
        +updateLights(int deltaTime) void
        +isGreen(Direction direction) bool
        -setLights(LightState ns, LightState ew) void
    }

    class Intersection {
        -int id
        -map~Direction, Road*~ roads
        -LightController* lightController
        +Intersection(int id, map~Direction, Road*~ roads, LightController* lightController)
        +getId() int
        +update(int deltaTime) void
        +addCar(Direction direction, Car car) void
        +getRoad(Direction direction) Road*
        +getLightController() LightController*
        -moveCars(Direction from, Direction to) void
    }

    %% Relationships
    Lane "1" *-- "0..*" Car : contains (queue)
    Lane "0..*" --> "1" Road : belongs to

    Road "1" *-- "2" Lane : owns (queueLane + arrivalLane)
    Road --> Direction : uses

    TrafficLight --> "1" Road : associated with
    TrafficLight --> LightState : uses
    LightController ..> TrafficLight : friend (accesses setState)

    LightController "1" *-- "4" TrafficLight : manages
    LightController --> Phase : uses
    LightController --> Direction : uses

    Intersection "1" *-- "4" Road : owns
    Intersection "1" *-- "1" LightController : owns
    Intersection --> Direction : uses
```

## Notes

- **Friendship**: `LightController` is declared a `friend` of `TrafficLight`, giving it exclusive access to the private `setState()` method (shown as a dashed dependency).
- **Containment hierarchy**: `Intersection` → `Road` → `Lane` → `Car`
- **Traffic flow**: On each `update()` tick, `Intersection` checks the green directions via `LightController` and moves one car from a road's `queueLane` to the opposite road's `arrivalLane`.
- **Phase cycle**: `NS_GREEN` → `NS_YELLOW` → `EW_GREEN` → `EW_YELLOW` → repeat, governed by `GREEN_DURATION=10`, `YELLOW_DURATION=3`, `RED_DURATION=10`.
