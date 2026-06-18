#pragma once
#include "raylib.h"
#include "paddle.hpp"
#include "ball.hpp"
#include "brick.hpp"
#include <vector>

using namespace std;

struct Particle{
    Vector2 position;
    Vector2 velocity;
    float lifetime;
    Color color;
    float size;
};

enum class GameState
{
    PLAYING,
    LEVEL_COMPLETE,
    GAME_OVER
};

class Game{
    private:
        int lives, currentLevel, score;
        bool ballLaunched, levelComplete;//temp test

        Paddle paddle;
        Ball ball;
        vector<Brick> bricks;
        vector<Particle> particles;
        GameState currentState;

        Sound paddleHitSound, brickBreakSound, levelCompleteSound, gameOverSound;



        bool checkCollisionWithPaddle();
        bool isLevelComplete() const;

        float getNormalizedImpactOffset();

        void initSounds();
        void handleCollisionWithPaddle();
        void handleBrickCollisions();
        void addBrick(int row, int col);
        void loadLevel(int levelNumber);
        void loadLevel1();
        void loadLevel2();
        void loadLevel3();
        void restartGame();
        void resetLives();
        void resetScore();
        void resetLevel();
        void spawnBrickParticles(Rectangle brickBounds, Color brickColor);
        void updateParticles(float dt);
        void drawParticles();

        Color getBrickColor(int row);

    public:
        Game();
        void handleInput();
        void update();
        void draw();
        ~Game();
};