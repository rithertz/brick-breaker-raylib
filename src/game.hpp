#pragma once
#include "raylib.h"
#include "paddle.hpp"
#include "ball.hpp"
class Game{
    private:
        Paddle paddle;
        Ball ball;
        bool checkCollisionWithPaddle();
        void handleCollisionWithPaddle();
        float getNormalizedImpactOffset();
    public:
        Game();
        void handleInput();
        void update();
        void draw();
};