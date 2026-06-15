#pragma once
#include "raylib.h"
#include "paddle.hpp"
#include "ball.hpp"
#include "brick.hpp"
#include<vector>

using namespace std;

enum class GameState
{
    PLAYING,
    LEVEL_COMPLETE
};

class Game{
    private:
        Paddle paddle;
        Ball ball;
        vector<Brick> bricks;

        GameState currentState;

        bool checkCollisionWithPaddle();
        void handleCollisionWithPaddle();
        float getNormalizedImpactOffset();
        void initializeBricks();
        void handleBrickCollisions();
        bool isLevelComplete() const;
    public:
        Game();
        void handleInput();
        void update();
        void draw();
};