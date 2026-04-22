CXX          = g++
SFML_INCLUDE = -I/opt/homebrew/Cellar/sfml/3.0.2/include
SFML_LIB     = -L/opt/homebrew/Cellar/sfml/3.0.2/lib
CXXFLAGS     = -std=c++17 -Wall -I./src/backend -I./src/visuals $(SFML_INCLUDE)
SFMLFLAGS    = $(SFML_LIB) -lsfml-graphics -lsfml-window -lsfml-system

BACKEND_SRCS = src/backend/Car.cpp src/backend/Lane.cpp src/backend/Road.cpp \
               src/backend/TrafficLight.cpp src/backend/LightController.cpp \
               src/backend/Intersection.cpp
BACKEND_OBJS = $(BACKEND_SRCS:.cpp=.o)

VISUAL_SRCS  = src/visuals/Renderer.cpp src/visuals/main_sfml.cpp
VISUAL_OBJS  = $(VISUAL_SRCS:.cpp=.o)

all: traffic_sim traffic_visual

traffic_sim: $(BACKEND_OBJS) src/backend/main.o
	$(CXX) $(CXXFLAGS) -o traffic_sim $(BACKEND_OBJS) src/backend/main.o

traffic_visual: $(BACKEND_OBJS) $(VISUAL_OBJS)
	$(CXX) $(CXXFLAGS) -o traffic_visual $(BACKEND_OBJS) $(VISUAL_OBJS) $(SFMLFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(BACKEND_OBJS) src/backend/main.o $(VISUAL_OBJS) \
	      traffic_sim traffic_visual
