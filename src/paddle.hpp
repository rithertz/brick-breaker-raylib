#pragma once
#include "raylib.h"

class Paddle{
    private:
        float speed;  // pixels per second , not pixel per frame

        // Paddle dimension constants.
        const float defaultWidth = 150.0f;
        const float expandedWidth = 220.0f;
        // Paddle speed constants.
        const float defaultSpeed = 500.0f;

        Rectangle bounds;
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
        void setSpeed(float speed);
        void activateSpeedBoost(float boost);
        void deactivateSpeedBoost();

        Rectangle getBounds() const;
};