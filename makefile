CXX          = g++
SFML_INCLUDE = -I/opt/homebrew/Cellar/sfml/3.0.2/include
SFML_LIB     = -L/opt/homebrew/Cellar/sfml/3.0.2/lib
CXXFLAGS     = -std=c++17 -Wall -I./src -I./src/visuals $(SFML_INCLUDE)
SFMLFLAGS    = $(SFML_LIB) -lsfml-graphics -lsfml-window -lsfml-system

BACKEND_SRCS = src/Car.cpp src/Lane.cpp src/Road.cpp \
               src/TrafficLight.cpp src/LightController.cpp \
               src/Intersection.cpp
BACKEND_OBJS = $(BACKEND_SRCS:.cpp=.o)

traffic_sim: $(BACKEND_OBJS) src/main.o
	$(CXX) $(CXXFLAGS) -o traffic_sim $(BACKEND_OBJS) src/main.o

traffic_visual: $(BACKEND_OBJS) src/visuals/Renderer.o src/visuals/main_sfml.o
	$(CXX) $(CXXFLAGS) -o traffic_visual \
	$(BACKEND_OBJS) src/visuals/Renderer.o src/visuals/main_sfml.o \
	$(SFMLFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(BACKEND_OBJS) src/main.o \
	      src/visuals/Renderer.o src/visuals/main_sfml.o \
	      traffic_sim traffic_visual