#pragma once
#include "raylib.h"
#include "paddle.hpp"
#include "ball.hpp"
#include "brick.hpp"
#include<vector>

using namespace std;

class Game{
    private:
        Paddle paddle;
        Ball ball;
        vector<Brick> bricks;
        bool checkCollisionWithPaddle();
        void handleCollisionWithPaddle();
        float getNormalizedImpactOffset();
        void initializeBricks();
        void handleBrickCollisions() ;
    public:
        Game();
        void handleInput();
        void update();
        void draw();
};