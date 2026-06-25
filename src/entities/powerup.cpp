#include "powerup.hpp"

// Power-up constants
const Color EXPAND_PADDLE_COLOR = {100, 200, 255, 255}; // Electric Blue
const Color EXTRA_LIFE_COLOR = {80, 255, 140, 255}; // Neon Green
const Color OVERDRIVE_COLOR = {255, 170, 60, 255}; // Energy Orange

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
            return EXPAND_PADDLE_COLOR;
        case PowerUpType::EXTRA_LIFE:
            return EXTRA_LIFE_COLOR;
        case PowerUpType::OVERDRIVE:
            return OVERDRIVE_COLOR;
        default:
            return RAYWHITE;
    }
}

void PowerUp::draw() const
{   
    //Capsule
    DrawRectangleRounded(bounds, 0.5f, 8, getColor());
    DrawRectangleRoundedLinesEx(bounds, 0.5f, 8, 2.0f, Fade(RAYWHITE, 0.6f));
    // Top glossy highlight.
    DrawRectangleRounded({bounds.x + 2, bounds.y + 2, bounds.width - 4, 6}, 0.4f, 4, Fade(RAYWHITE, 0.25f));

    const char* symbol;
    switch(type){
        case PowerUpType::EXPAND_PADDLE:
            symbol  = "E";
            break;
        case PowerUpType::EXTRA_LIFE:
            symbol = "+";
            break;
        case PowerUpType::OVERDRIVE:
            symbol = ">>";
            break;
    }
    int width = MeasureText(symbol, 20);

    DrawText(symbol, bounds.x + bounds.width / 2 - width / 2, bounds.y + 3, 20, Fade(BLACK, 0.8f));
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
