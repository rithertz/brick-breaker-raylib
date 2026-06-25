#pragma once
#include "raylib.h"

class Paddle{
    private:
        bool speedBoosted;
        float speed;  // pixels per second , not pixel per frame

        // Paddle dimension constants.
        const float defaultWidth = 150.0f;
        const float expandedWidth = 225.0f;
        // Paddle speed constants.
        const float defaultSpeed = 500.0f;
        // OVERDRIVE constants
        const Color OVERDRIVE_COLOR = {255, 170, 50, 255};
        const Color OVERDRIVE_SPARK = {255, 240, 180, 255};

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
        void drawOverdriveEffect(float fraction) const;
        void drawOverdriveGlow(float fraction) const;
        void drawOverdriveBorder(float fraction, Vector2 &current) const;
        void drawOverdriveSpark(Vector2 current, float fraction) const;
        void drawOverdriveTrail() const;
        
        Color getOverdriveColor(float fraction) const;

        Rectangle getBounds() const;
};