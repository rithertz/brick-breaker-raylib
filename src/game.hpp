#pragma once
#include "paddle.hpp"
#include "ball.hpp"
class Game{
    private:
        Paddle paddle;
        Ball ball;
    public:
        Game();
        void handleInput();
        void update();
        void draw();
};