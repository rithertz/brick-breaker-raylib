#pragma once

#include "raylib.h"

enum class PowerUpType{
    EXPAND_PADDLE,
    EXTRA_LIFE
};

class PowerUp{
    private:
        bool active;
        float fallSpeed;
        Rectangle bounds;
        PowerUpType type;
    public:
        PowerUp(Rectangle bounds, PowerUpType type);
        void update(float dt);
        void draw() const;
        Rectangle getBounds() const;
        PowerUpType getType() const;
        bool isActive() const;
        void collect();
        Color getColor() const;
};