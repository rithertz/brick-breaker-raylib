#pragma once
#include<raylib.h>

class Ball{
    private:
        float radius, launchSpeed;
        void handleCollision();

        Vector2 position, previousPosition, velocity;
        Color ballColor;

    public:
        Ball(Vector2 initialPosition, Vector2 intialVelocity);

        bool isMovingDown() const;
        bool isMovingRight() const;
        bool isMovingLeft() const;
        bool isMovingUp() const;
        bool isOutOfBounds() const;

        float getRadius();

        void drawBall() const;
        void update(float dt);
        void invertXVelocity();
        void invertYVelocity();
        void handlePaddleCollision(float paddleTopY, float normalizedImpactOffset);
        void revertToPreviousPosition();
        void reset();
        void setPosition(Vector2 newPosition);
        void launch(float paddleCenterX);
        void increaseLaunchSpeed(float amount);
        void resetLaunchSpeed();

        Vector2 getPreviousPosition() const;
        Vector2 getPosition();
};