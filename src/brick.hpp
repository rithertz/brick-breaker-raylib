#pragma once
#include "raylib.h"
class Brick{
    private:
        Rectangle bounds;
        bool alive;
        Color brickColor;
    public:
        Brick(Rectangle bounds);
        void draw() const;
        bool isAlive() const;
        Rectangle getBounds() const;
        void destroy();
};