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
    LEVEL_COMPLETE,
    GAME_OVER
};

class Game{
    private:        
        Paddle paddle;
        Ball ball;
        int lives;
        bool ballLaunched;
        int currentLevel;
        bool levelComplete;//temp test
        int score;
        vector<Brick> bricks;
        GameState currentState;

        bool checkCollisionWithPaddle();
        void handleCollisionWithPaddle();
        float getNormalizedImpactOffset();
        void handleBrickCollisions();
        bool isLevelComplete() const;
        void addBrick(int row, int col);
        void loadLevel(int levelNumber);
        void loadLevel1();
        void loadLevel2();
        void loadLevel3();
        void restartGame();
        void resetLives();
        void resetScore();
        void resetLevel();
    public:
        Game();
        void handleInput();
        void update();
        void draw();
};