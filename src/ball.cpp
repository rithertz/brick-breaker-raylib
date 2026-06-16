#include "ball.hpp"
#include "raymath.h"
#include<iostream>
using namespace std;
void Ball::handleCollision()
{
    // Checks screen boundaries and reverses ball direction when hitting walls.
    // Also clamps position to prevent the ball from getting stuck outside the screen.
    if(position.x - radius < 0){
        position.x = radius;
        invertXVelocity();
    }
    if(position.x + radius > GetScreenWidth()){
        position.x = GetScreenWidth() - radius;
        invertXVelocity();
    }
    // if(position.y + radius > GetScreenHeight()){
    //     position.y = GetScreenHeight() - radius;
    //     invertYVelocity();
    // }
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
    previousPosition = position;
}

void Ball::drawBall() const
{
    DrawCircleV(position, radius, ballColor);
    DrawCircleV({position.x - radius/4, position.y - radius/4}, radius/5, Fade(RAYWHITE, 0.5f)); // Small Highlight
}

void Ball::update(float dt)
{   
    previousPosition = position;
    // Update position - frame independent
    position.x += velocity.x * dt;
    position.y += velocity.y * dt;
    handleCollision();
}

Vector2 Ball::getPosition()
{
    return position;
}

float Ball::getRadius()
{
    return radius;
}

void Ball::handlePaddleCollision(float paddleTopY, float normalizedImpactOffset)
{   
    // place ball above paddle to avoid unwanted effects
    position.y = paddleTopY - radius;

    // magnitude of velocity vector
    float ballSpeed = Vector2Length(velocity);

    // update Vx & Vy
    float maxHorizonatalInfluence= 0.95;
    velocity.x = maxHorizonatalInfluence * normalizedImpactOffset * ballSpeed; 
    velocity.y = std::sqrt(ballSpeed * ballSpeed - velocity.x * velocity.x);
    invertYVelocity();
}
bool Ball::isMovingDown() const
{
    return velocity.y > 0;
}

bool Ball::isMovingRight() const
{
    return velocity.x > 0;
}

bool Ball::isMovingLeft() const
{
    return velocity.x < 0;
}

bool Ball::isMovingUp() const
{
    return velocity.y < 0;
}

void Ball::revertToPreviousPosition()
{
    position = previousPosition;
}

Vector2 Ball::getPreviousPosition() const
{
    return previousPosition;
}

bool Ball::isOutOfBounds() const
{
    return position.y - radius > GetScreenHeight();
}

void Ball::reset()
{
    velocity = {300, 300};
    previousPosition = position;
}

void Ball::setPosition(Vector2 newPosition)
{
    position = newPosition;
    previousPosition = position;
}

void Ball::launch(float paddleCenterX)
{   
    if(paddleCenterX > GetScreenWidth() / 2){
        velocity = {-300, -300};
    }
    else{
        velocity = {300, -300};
    }
}
