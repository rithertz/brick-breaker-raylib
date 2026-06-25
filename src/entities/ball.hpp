#pragma once
#include<raylib.h>

class Ball{
    private:
        float radius, launchSpeed, previousSpeed;

        const float defaultLaunchSpeed = 300.0f;
        const float boostedLaunchSpeed = 375.0f;
        
        Vector2 position, previousPosition, velocity;
        Color ballColor;
        
        public:
        Ball(Vector2 initialPosition, Vector2 intialVelocity);
        
        bool isMovingDown() const;
        bool isMovingRight() const;
        bool isMovingLeft() const;
        bool isMovingUp() const;
        bool isOutOfBounds() const;
        bool isSpeedBoosted() const;
        
        float getRadius();
        
        void drawBall() const;
        void update(float dt);
        void invertXVelocity();
        void invertYVelocity();
        void handlePaddleCollision(float paddleTopY, float normalizedImpactOffset);
        void handleCollision();
        void revertToPreviousPosition();
        void reset();
        void setPosition(Vector2 newPosition);
        void launch(float paddleCenterX);
        void increaseLaunchSpeed(float amount);
        void resetLaunchSpeed();
        void activateSpeedBoost(float boost);
        void deactivateSpeedBoost();

        Vector2 getPreviousPosition() const;
        Vector2 getPosition();
};