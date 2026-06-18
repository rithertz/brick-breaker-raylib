#pragma once
#include "raylib.h"
class Brick{
    private:
        Rectangle bounds;
        bool alive;
        Color brickColor;
    public:
        Brick(Rectangle bounds, Color brickColor);
        void draw() const;
        bool isAlive() const;
        void destroy();
        Rectangle getBounds() const;
        Color getColor() const;
};