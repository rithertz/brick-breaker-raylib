#pragma once
#include "raylib.h"

class Paddle{
    private:
        Rectangle bounds;
        float speed;  // pixels per second , not pixel per frame
        Color paddleColor;     
    public:
        Paddle(Rectangle bounds);
        void drawPaddle() const;
        void moveLeft(float dt);
        void moveRight(float dt);
        Rectangle getBounds() const;
        void reset();
};