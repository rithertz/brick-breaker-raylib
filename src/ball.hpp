#pragma once
#include<raylib.h>

class Ball{
    private:
        Vector2 position, velocity;
        Color ballColor;
        float radius;
        void handleCollision();
        void invertXVelocity();
        void invertYVelocity();
    public:
        Ball(Vector2 initialPosition, Vector2 intialVelocity);
        void drawBall() const;
        void update(float dt);
};