#pragma once
#include "raylib.h"
#include "paddle.hpp"
#include "ball.hpp"
#include "brick.hpp"
#include "powerup.hpp"
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
    MAIN_MENU,
    PLAYING,
    PAUSED,
    LEVEL_COMPLETE,
    GAME_OVER
};

class Game{
    private:
        int lives, currentLevel, score, highScore, bricksDestroyed, strongBricksDestroyed;
        bool ballLaunched, levelComplete, paddleExpanded, overdriveActive;
        float screenShakeTime, screenShakeStrength, paddleExpandTimer, overdriveTimer;

        Paddle paddle;
        Ball ball;
        vector<Brick> bricks;
        vector<Particle> particles;
        vector<PowerUp> powerUps;
        GameState currentState;

        Sound paddleHitSound, brickBreakSound, levelCompleteSound, gameOverSound, armorBreakSound, victorySound;

        Camera2D camera;

        bool checkCollisionWithPaddle();
        bool isLevelComplete() const;
        bool isStrongBrickLevel1(int row, int col) const;
        bool isStrongBrickLevel2(int row, int col) const;
        bool isStrongBrickLevel3(int row, int col) const;
        bool checkPowerUpCollision(const PowerUp& powerUp) const;

        float getNormalizedImpactOffset();    

        void initSounds();
        void handleCollisionWithPaddle();
        void handleBrickCollisions();
        void addNormalBrick(int row, int col);
        void addStrongBrick(int row, int col);
        void addBrick(int row, int col, BrickType type);
        void loadLevel(int levelNumber);
        void loadLevel1();
        void loadLevel2();
        void loadLevel3();
        void restartGame();
        void resetLives();
        void resetScore();
        void resetLevel();
        void resetBricksDestroyed();
        void spawnBrickParticles(Rectangle brickBounds, Color brickColor);
        void spawnArmorBreakParticles(Rectangle bounds, Color color);
        void updateParticles(float dt);
        void drawParticles();
        void startScreenShake(float duration, float strength);
        void initializeCamera();
        void drawOverlay(Color color);
        void handlePauseInput();
        void handleRestartInput();
        void handlePaddleInput(float dt);
        void handleBallLaunchInput();
        void updateHighScore();
        void drawCenteredText(const char* text, int y, int fontSize, Color color);
        void drawEndScreen(const char* title, Color titleColor, const char* instruction);
        void drawHUDText(const char* text, int centerX, Color color);
        void drawPauseScreen();
        void loadHighScore();
        void saveHighScore() const;
        void handleMainMenuInput();
        void drawMainMenu();
        void spawnPowerUp(Vector2 position);
        void updatePowerUps(float dt);
        void applyPowerUp(PowerUpType type);
        void updateExpandedPaddle(float dt);
        void drawPowerUpStatus() const;
        void updateOverdrive(float dt);
        void applyExtraLife();
        void applyExpandPaddle();
        void applyOverdrive();

        Color getBrickColor(int row);

        PowerUpType getRandomPowerUpType() const;

    public:
        Game();
        void handleInput();
        void update();
        void draw();
        ~Game();
};