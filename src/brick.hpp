#pragma once
#include "raylib.h"

enum class BrickType{
    NORMAL, STRONG
};

class Brick{
    private:
        bool alive;
        int health;

        Rectangle bounds;
        Color brickColor;
        BrickType type;


        bool isStrong() const;
        void drawBase() const;
        void drawArmor() const;
        void drawGloss() const;

        Color getFillColor() const;
        Color getBorderColor() const;


    public:
        Brick(Rectangle bounds, Color brickColor, BrickType type);
        bool isAlive() const;
        bool isDestroyed() const;
        void draw() const;
        void takeDamage();
        int getHealth() const;
        Rectangle getBounds() const;
        Color getColor() const;
        BrickType getType() const;
};