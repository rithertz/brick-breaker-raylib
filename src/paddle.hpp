#pragma once
#include "raylib.h"

class Paddle{
    private:
        Rectangle bounds;
        float speed;  // pixels per second , not pixel per frame
        const float defaultWidth = 150.0f;
        const float expandedWidth = 220.0f;
        Color paddleColor;     
    public:
        Paddle(Rectangle bounds);
        bool isExpanded() const;
        void drawPaddle() const;
        void moveLeft(float dt);
        void moveRight(float dt);
        void reset();
        void expand();
        void resetSize();
        Rectangle getBounds() const;
};