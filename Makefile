PROJECT = brick_breaker

CXX = g++

CXXFLAGS = -Wall -std=c++17 \
           -I./src \
           -I./src/game \
           -I./src/entities \
           -I./src/managers \
           -I./src/states \
           -I./lib/include \
           -IC:/raylib/raylib/src

LDFLAGS = -L./lib -lraylib -lopengl32 -lgdi32 -lwinmm

SRC = \
	src/main.cpp \
	src/game/game.cpp \
	src/entities/paddle.cpp \
	src/entities/ball.cpp \
	src/entities/brick.cpp \
	src/entities/powerup.cpp

OBJ = $(SRC:.cpp=.o)

all: $(PROJECT)

$(PROJECT): $(OBJ)
	$(CXX) $(OBJ) -o $(PROJECT).exe $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f src/*.o
	rm -f src/game/*.o
	rm -f src/entities/*.o
	rm -f brick_breaker.exe