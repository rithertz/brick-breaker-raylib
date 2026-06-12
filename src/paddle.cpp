#include "paddle.hpp"

Paddle::Paddle(Rectangle bounds)
{
    this->bounds = bounds;
    paddleColor = {80, 220, 255, 255}; // cyanish
    speed = 500.0f;                    // Moves across screen in apprx 2.5 s
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
