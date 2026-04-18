CXX  = g++
CXXFLAGS = -std=c++17 -Wall -I./src

SRCS = src/Car.cpp src/Lane.cpp src/Road.cpp \
       src/TrafficLight.cpp src/LightController.cpp \
       src/Intersection.cpp src/main.cpp

OBJS = $(SRCS:.cpp=.o)

traffic_sim: $(OBJS)
	$(CXX) $(CXXFLAGS) -o traffic_sim $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) traffic