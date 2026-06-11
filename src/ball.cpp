#include "ball.hpp"

void Ball::handleCollision()
{
    if(position.x - radius < 0){
        position.x = radius;
        invertXVelocity();
    }
    if(position.x + radius > GetScreenWidth()){
        position.x = GetScreenWidth() - radius;
        invertXVelocity();
    }
    if(position.y + radius > GetScreenHeight()){
        position.y = GetScreenHeight() - radius;
        invertYVelocity();
    }
    if(position.y - radius < 0){
        position.y = radius;
        invertYVelocity();
    }
}

void Ball::invertXVelocity()
{
    velocity.x *= -1;
}

void Ball::invertYVelocity()
{
    velocity.y *= -1;
}

Ball::Ball(Vector2 initialPosition, Vector2 intialVelocity) : position(initialPosition), velocity(intialVelocity)
{
    radius = 10.0f;
    ballColor = {240, 245, 255, 255};
}

void Ball::drawBall() const
{
    DrawCircleV(position, radius, ballColor);
    DrawCircleV({position.x - radius/4, position.y - radius/4}, radius/5, Fade(RAYWHITE, 0.5f)); // Small Highlight
}

void Ball::update(float dt)
{
    // Update position - frame independent
    position.x += velocity.x * dt;
    position.y += velocity.y * dt;
    handleCollision();
}
