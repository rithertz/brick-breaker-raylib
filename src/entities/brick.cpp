#include "brick.hpp"

Brick::Brick(Rectangle bounds, Color brickColor, BrickType type) : alive(true), bounds(bounds), brickColor(brickColor), type(type)
{
    health = (type == BrickType::NORMAL) ? 1 : 2;
}


bool Brick::isStrong() const
{
    return type == BrickType::STRONG;
}

void Brick::drawBase() const
{
    DrawRectangleRounded(bounds, 0.15f, 8, getFillColor());
    DrawRectangleRoundedLinesEx(bounds, 0.15f, 8, 2.5f, getBorderColor());
}

void Brick::drawArmor() const
{
    if(health != 2){
        return;
    }
    // Reinforced outer border indicating extra durability
    DrawRectangleRoundedLinesEx(bounds, 0.15f, 8, 4.0f, getBorderColor());
}

void Brick::drawGloss() const
{
    if(health != 2){
        return;
    }
    // Top highlight strip
    DrawRectangleRounded({bounds.x + 5, bounds.y + 3, bounds.width - 8, 8}, 0.2f, 4, Fade(RAYWHITE, 0.45f));
    // Subtle Inner glow
    DrawRectangleRounded({bounds.x + 3, bounds.y + 3, bounds.width - 6, bounds.height - 6}, 0.15f, 8, Fade(RAYWHITE, 0.08f));
    // Reflection spot on top-left corner
    DrawCircle(bounds.x + 12, bounds.y + 8, 4, Fade(RAYWHITE, 0.35f));
    //Botom Shadow for depth
    DrawRectangleRounded({bounds.x + 3, bounds.y + bounds.height - 7, bounds.width - 6, 4}, 0.2f, 4, Fade(BLACK, 0.25f));
}

Color Brick::getFillColor() const
{
    // Full-health strong brick
    if(isStrong() && health == 2){
        return {(unsigned char)(brickColor.r * 0.65f), (unsigned char)(brickColor.g * 0.65f), (unsigned char)(brickColor.b * 0.65f), 255};
    }
    // Normal brick OR damaged strong brick
    return brickColor;
}

Color Brick::getBorderColor() const
{
    // Darkest shade of brick color
    return {(unsigned char)(brickColor.r * 0.45f), (unsigned char)(brickColor.g * 0.45f), (unsigned char)(brickColor.b * 0.45f), 255};
}
void Brick::draw() const
{   
    if(!alive){
        return;
    }

    drawBase();

    if(isStrong() && health == 2){
        drawArmor();
        drawGloss();
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


void Brick::takeDamage()
{
    health--;
    if(health <= 0){
        alive = false;
    }
}

bool Brick::isDestroyed() const
{
    return !alive;
}

int Brick::getHealth() const
{
    return health;
}

Color Brick::getColor() const
{
    return brickColor;
}

BrickType Brick::getType() const
{
    return type;
}
