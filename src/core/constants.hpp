#pragma once

#include "raylib.h"

namespace Colors
{
    constexpr Color SCORE_COLOR = {255, 215, 100, 255};
    constexpr Color LIVES_COLOR = {255, 120, 120, 255};
    constexpr Color LEVEL_COLOR = {80, 220, 255, 255};
    constexpr Color GAME_OVER_COLOR = {255, 80, 80, 255};
    constexpr Color LEVEL_COMPLETE_COLOR = {120, 255, 140, 255};
    constexpr Color INSTRUCTION_COLOR = {200, 210, 230, 255};
    constexpr Color OVERLAY_COLOR = {8, 15, 25, 220};
    constexpr Color BOOST_COLOR = {170, 220, 255, 255};
    constexpr Color OVERDRIVE_COLOR = {255, 180, 60, 255};


    constexpr Color RED_BRICK = {255, 100, 100, 255};   // Red
    constexpr Color ORANGE_BRICK = {255, 170, 80, 255};    // Orange
    constexpr Color YELLOW_BRICK = {255, 230, 100, 255};   // Yellow
    constexpr Color GREEN_BRICK = {120, 255, 140, 255};   // Green
    constexpr Color CYAN_BRICK = {80, 220, 255, 255};    // Cyan
    constexpr Color PURPLE_BRICK = {180, 140, 255, 255};   // Purple
}

namespace Gameplay
{
    constexpr int MAX_LIVES = 3;
    constexpr float OVERDRIVE_DURATION = 15.0f;
    constexpr float OVERDRIVE_BALL_MULTIPLIER = 1.25f;
    constexpr float OVERDRIVE_PADDLE_MULTIPLIER = 1.50f;
    constexpr int OVERDRIVE_SCORE_MULTIPLIER = 2;

    constexpr float LEVEL_SPEED_INCREASE = 30.0f;
}

namespace Particles
{
    constexpr int BRICK_COUNT = 8;
    constexpr int ARMOR_COUNT = 4;

    constexpr float BRICK_LIFETIME = 0.5f;
    constexpr float ARMOR_LIFETIME = 0.3f;
}

namespace PowerUps
{
    constexpr float PADDLE_EXPAND_DURATION = 10.0f;
    constexpr float MAX_EXPAND_DURATION = 90.0f;

    constexpr int EXTRA_LIFE_CAP = 6;

    constexpr int SPAWN_CHANCE = 30;
}

namespace Scoring
{
    constexpr int BRICK = 100;
    constexpr int STRONG_HIT = 50;
    constexpr int LEVEL_COMPLETE = 500;
}