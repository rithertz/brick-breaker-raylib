#pragma once
#include<raylib.h>

class Ball{
    private:
        Vector2 position, previousPosition, velocity;
        Color ballColor;
        float radius;

        void handleCollision();
        

    public:
        Ball(Vector2 initialPosition, Vector2 intialVelocity);
        void drawBall() const;
        void update(float dt);
        void invertXVelocity();
        void invertYVelocity();
        Vector2 getPosition();
        float getRadius();
        void handlePaddleCollision(float paddleTopY, float normalizedImpactOffset);
        bool isMovingDown() const;
        bool isMovingRight() const;
        bool isMovingLeft() const;
        bool isMovingUp() const;
        void revertToPreviousPosition();
        Vector2 getPreviousPosition() const;
};