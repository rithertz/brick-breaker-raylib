#include "powerup.hpp"

PowerUp::PowerUp(Rectangle bounds, PowerUpType type) : active(true), fallSpeed(250.0f), bounds(bounds), type(type)
{

}

void PowerUp::update(float dt)
{
    bounds.y += fallSpeed * dt;
}

Color PowerUp::getColor() const
{
    switch(type){
        case PowerUpType::EXPAND_PADDLE:
            return SKYBLUE;
        case PowerUpType::EXTRA_LIFE:
            return GREEN;
        default:
            return RAYWHITE;
    }
}

void PowerUp::draw() const
{   
    //Capsule
    DrawRectangleRounded(bounds, 0.5f, 8, getColor());
    DrawRectangleRoundedLines(bounds, 0.5f, 8, Fade(RAYWHITE, 0.4f));

    const char* symbol;
    symbol = (type == PowerUpType::EXPAND_PADDLE) ? "E" : "+"; 
    int width = MeasureText(symbol, 20);

    DrawText(symbol, bounds.x + bounds.width / 2 - width / 2, bounds.y + 3, 20, BLACK);
}

Rectangle PowerUp::getBounds() const
{
    return bounds;
}

bool PowerUp::isActive() const
{
    return active;
}

PowerUpType PowerUp::getType() const
{
    return type;
}

void PowerUp::collect()
{
    active = false;
}
