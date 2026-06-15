#include "brick.hpp"

Brick::Brick(Rectangle bounds)
{
    this->bounds = bounds;
    // brickColor = {255, 120, 80, 255};
    brickColor = {247, 114, 47, 255};
    alive = true;
}

void Brick::draw() const
{
    if(alive){
        DrawRectangleRounded(bounds, 0.15f, 8, brickColor);
        DrawRectangleRoundedLines(bounds, 0.12f, 8, Fade(WHITE, 0.15f));
    }
}

bool Brick::isAlive() const
{
    return alive;
}

Rectangle Brick::getBounds() const
{
    return bounds;
}

void Brick::destroy()
{
    alive = false;
}
