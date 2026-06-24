#include "paddle.hpp"

Paddle::Paddle(Rectangle bounds)
{
    this->bounds = bounds;
    paddleColor = {80, 220, 255, 255}; // cyanish
    speed = defaultSpeed; // Moves across screen in apprx 2.5 s
}

void Paddle::drawPaddle() const
{
    DrawRectangleRounded(bounds, 0.3f, 16, paddleColor);
}

void Paddle::moveLeft(float dt)
{
    /*
        dt is time elapsed since the last frame
        ∴ distance to be moved any time key is pressed is = speed * time
    */
    bounds.x -= speed * dt;

    // Handle boundary condition
    if (bounds.x < 0)
    {

        bounds.x = 0;
    }
}

void Paddle::moveRight(float dt)
{
    /*
        dt is time elapsed since the last frame
        ∴ distance to be moved any time key is pressed is = speed * time
    */
    bounds.x += speed * dt;

    // Handle boundary condition
    if (bounds.x + bounds.width > GetScreenWidth())
    {
        bounds.x = GetScreenWidth() - bounds.width;
    }
}

Rectangle Paddle::getBounds() const
{
    return bounds;
}

void Paddle::setSpeed(float speed)
{
    this->speed = speed;
}

void Paddle::activateSpeedBoost()
{
    speed = boostedSpeed;
}

void Paddle::deactivateSpeedBoost()
{
    speed = defaultSpeed;
}


void Paddle::reset()
{
    bounds.x = (GetScreenWidth() - bounds.width) / 2.0f;
}

void Paddle::expand()
{
    // Preserve paddle center while increasing width.
    float centerX = bounds.x + bounds.width / 2.0f;

    bounds.width = expandedWidth;
    bounds.x = centerX - bounds.width / 2.0f;
}

void Paddle::resetSize()
{
    // Restore to default paddle width.
    float centerX = bounds.x + bounds.width / 2.0f;

    bounds.width = defaultWidth;
    bounds.x = centerX - bounds.width / 2.0f;
}

bool Paddle::isExpanded() const
{
    return bounds.width == expandedWidth;
}

bool Paddle::isSpeedBoosted() const
{
    return (speed == boostedSpeed);
}
