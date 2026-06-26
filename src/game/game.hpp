/*
    game.hpp
    Central game controller.

    Responsibilities:
    - Game state management
    - Gameplay rules
    - Entity ownership
    - Update loop
    - Rendering coordination
*/

#pragma once

#include "raylib.h"
#include <vector>

#include "../entities/paddle.hpp"
#include "../entities/ball.hpp"
#include "../entities/brick.hpp"
#include "../entities/powerup.hpp"

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
        // Game state
        GameState currentState;

        // Gameplay statistics
        int lives, currentLevel, score, highScore, bricksDestroyed, strongBricksDestroyed;

        // Gameplay flags
        bool ballLaunched, levelComplete, paddleExpanded, overdriveActive;

        // Timers and effects
        float screenShakeTime, screenShakeStrength, paddleExpandTimer, overdriveTimer;

        // Entities
        Paddle paddle;
        Ball ball;

        std::vector<Brick> bricks;
        std::vector<Particle> particles;
        std::vector<PowerUp> powerUps;

        // Camera
        Camera2D camera;

        // Audio
        Sound paddleHitSound, brickBreakSound, levelCompleteSound, gameOverSound, armorBreakSound, victorySound;

        /* Member-Functions */

        // Input
        void handlePauseInput();
        void handleRestartInput();
        void handlePaddleInput(float dt);
        void handleBallLaunchInput();
        void handleMainMenuInput();

        //Audio
        void initSounds();
        void loadSounds();
        void adjustVolume();
        void unloadSounds();

        // Collision
        bool checkCollisionWithPaddle();
        bool checkPowerUpCollision(const PowerUp& powerUp) const;
        void handleCollisionWithPaddle();
        void handleBrickCollisions();

        // Level Management
        bool isLevelComplete() const;
        void loadLevel(int levelNumber);
        void loadLevel1();
        void loadLevel2();
        void loadLevel3();

        // Brick helpers
        void addNormalBrick(int row, int col);
        void addStrongBrick(int row, int col);
        void addBrick(int row, int col, BrickType type);
        bool isStrongBrickLevel1(int row, int col) const;
        bool isStrongBrickLevel2(int row, int col) const;
        bool isStrongBrickLevel3(int row, int col) const;
        Color getBrickColor(int row);

        //Scoring
        int getScoreMultiplier() const;
        void increaseScore(int score);
        void resetScore();

        // Power-ups
        bool hasOverdrivePowerUp() const;
        void spawnPowerUp(Vector2 position);
        void updatePowerUps(float dt);
        void applyPowerUp(PowerUpType type);
        void updateExpandedPaddle(float dt);
        void drawPowerUpStatus() const;
        void updateOverdrive(float dt);
        void applyExtraLife();
        void applyExpandPaddle();
        void applyOverdrive();
        PowerUpType getRandomPowerUpType() const;

        // Particles
        void spawnBrickParticles(Rectangle brickBounds, Color brickColor);
        void spawnArmorBreakParticles(Rectangle bounds, Color color);
        void updateParticles(float dt);
        void drawParticles();

        // Persistence
        void loadHighScore();
        void saveHighScore() const;
        void updateHighScore();
        
        // UI
        void drawOverlay(Color color);
        void drawCenteredText(const char* text, int y, int fontSize, Color color);
        void drawEndScreen(const char* title, Color titleColor, const char* instruction);
        void drawHUDText(const char* text, int centerX, Color color);
        void drawPauseScreen();
        void drawMainMenu();        
        void displayGameStatus();//HUD

        // Visual Effects
        void startScreenShake(float duration, float strength);
        void initializeCamera();

        // Game Control
        void restartGame();
        void resetLives();
        void resetLevel();
        void resetBricksDestroyed();

        // Utility
        float getNormalizedImpactOffset();    

    public:
        Game();
        ~Game();
        void handleInput();
        void update();
        void draw();
};