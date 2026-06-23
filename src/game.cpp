#include "game.hpp"
#include "raymath.h"
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <string>
#include <fstream>

using namespace std;

// Game Constants
const Color SCORE_COLOR = {255, 215, 100, 255};
const Color LIVES_COLOR = {255, 120, 120, 255};
const Color LEVEL_COLOR = {80, 220, 255, 255};
const Color GAME_OVER_COLOR = {255, 80, 80, 255};
const Color LEVEL_COMPLETE_COLOR = {120, 255, 140, 255};
const Color INSTRUCTION_COLOR = {200, 210, 230, 255};
const Color OVERLAY_COLOR = {8, 15, 25, 220};
const Color BOOST_COLOR = {170, 220, 255, 255};
const int MAX_LIVES = 3;


Game::Game() : lives(MAX_LIVES), currentLevel(1), score(0), highScore(0), bricksDestroyed(0), strongBricksDestroyed(0),
               ballLaunched(false), levelComplete(false), paddleExpanded(false),
               screenShakeTime(0.0f), screenShakeStrength(0.0f), paddleExpandTimer(0.0f),
               paddle(Rectangle{580, 660, 150, 25}), ball({640, 630}, {900, 900}),
               currentState(GameState::MAIN_MENU)
{   
    initSounds();
    initializeCamera();
    loadHighScore();
    loadLevel(1);
}

void Game::initSounds()
{   
    // Load Sounds
    paddleHitSound = LoadSound("assets/sounds/paddle_hit.wav");
    brickBreakSound = LoadSound("assets/sounds/brick_break.wav");
    levelCompleteSound = LoadSound("assets/sounds/level_complete.wav"); 
    gameOverSound = LoadSound("assets/sounds/game_over.wav");
    armorBreakSound = LoadSound("assets/sounds/armor_break.wav");
    victorySound = LoadSound("assets/sounds/victory_sound.wav");
    

    // Adjust Volume
    SetSoundVolume(brickBreakSound, 0.5f);
    SetSoundVolume(paddleHitSound, 0.7f);
    SetSoundVolume(levelCompleteSound, 1.0f);
    SetSoundVolume(gameOverSound, 1.0f);
    SetSoundVolume(armorBreakSound, 0.7f);
    SetSoundVolume(victorySound, 0.7f);
}


bool Game::checkCollisionWithPaddle()
{
    // Check for collision && if ball is moving down && ball is above paddle
    if(!ballLaunched){
        return false;
    }

    return CheckCollisionCircleRec(ball.getPosition(),ball.getRadius(),paddle.getBounds())
        && ball.isMovingDown() 
        && ball.getPreviousPosition().y < paddle.getBounds().y;
}

void Game::handleCollisionWithPaddle()
{   
    PlaySound(paddleHitSound);
    startScreenShake(0.06f, 1.5f);
    ball.handlePaddleCollision(paddle.getBounds().y, getNormalizedImpactOffset());    
}

float Game::getNormalizedImpactOffset()
{
    //Calculate distance b/w ball center and paddle in x-axis 
    float paddleCenterX = paddle.getBounds().x + (paddle.getBounds().width) / 2.0;
    float ballCenterX = ball.getPosition().x;
    float displacement = ballCenterX - paddleCenterX;

    //divide by half-paddle-width and clamp b/w (-1,1)
    float temp = 2.0f * displacement / paddle.getBounds().width;
    float normalizedOffset = Clamp(temp, -1.0f, 1.0f);

    // Prevent vertical loops
    if(fabs(normalizedOffset) < 0.05f){
        normalizedOffset = (ball.isMovingRight()) ? 0.05f : -0.05f;
    }
    return normalizedOffset;
}

void Game::handleBrickCollisions()
{
    for(Brick& brick : bricks){
        if(brick.isAlive() && CheckCollisionCircleRec(ball.getPosition(), ball.getRadius(), brick.getBounds())){
            Rectangle bounds = brick.getBounds();

            Vector2 prevPos = ball.getPreviousPosition();
            float radius = ball.getRadius();

            float left   = bounds.x;
            float right  = bounds.x + bounds.width;

            ball.revertToPreviousPosition();

            if(prevPos.x + radius <= left){
                // Hit left side
                ball.invertXVelocity();
            }
            else if(prevPos.x - radius >= right){
                // Hit right side
                ball.invertXVelocity();
            }
            else{
                // Hit top or bottom
                ball.invertYVelocity();
            }

            brick.takeDamage();
            if(brick.getType() == BrickType::STRONG && brick.isAlive()){
                score += 50;
                PlaySound(armorBreakSound);
                spawnArmorBreakParticles(brick.getBounds(), brick.getColor());
                startScreenShake(0.04f, 2.0f);
            }
            if(!brick.isAlive()){
                bricksDestroyed++;
                if(brick.getType() == BrickType::STRONG){
                    strongBricksDestroyed++;
                }
                score += 100;

                spawnBrickParticles(brick.getBounds(), brick.getColor());
                PlaySound(brickBreakSound);
                startScreenShake(0.08f, 4.0f);

                // Spawn power-up with a small random chance (15%)
                if(rand() % 100 < 15){
                    Vector2 center = {brick.getBounds().x + brick.getBounds().width / 2, brick.getBounds().y + brick.getBounds().height / 2};
                    spawnPowerUp(center);
                }
            }
            updateHighScore();
            break;
        }
    }
}

void Game::addNormalBrick(int row, int col)
{
    addBrick(row, col, BrickType::NORMAL);
}

void Game::addStrongBrick(int row, int col)
{
    addBrick(row, col, BrickType::STRONG);
}

bool Game::isLevelComplete() const
{
    for(const Brick& brick : bricks){
        if(brick.isAlive()){
            return false;
        }
    }
    return true;
}

bool Game::isStrongBrickLevel1(int row, int col) const
{
    return (row == 3 && col == 5);
}

bool Game::isStrongBrickLevel2(int row, int col) const
{
    return (row == 2 && col == 3) || (row == 2 && col == 7) || (row == 3 && col == 5);
}

bool Game::isStrongBrickLevel3(int row, int col) const
{
    return ((row == 1 && col == 5) || (row == 2 && col == 4) || (row == 2 && col == 6) || (row == 3 && col == 0) || (row == 3 && col == 10));
}

bool Game::checkPowerUpCollision(const PowerUp &powerUp) const
{
    return CheckCollisionRecs(powerUp.getBounds(), paddle.getBounds());
}

void Game::addBrick(int row, int col, BrickType type)
{
    const int columns = 11;

    const float brickWidth = 90;
    const float brickHeight = 25;
    const float horizontalGap = 10;
    const float verticalGap = 10;

    float totalWidth = columns * brickWidth + (columns - 1) * horizontalGap;
    
    float startX = (GetScreenWidth() - totalWidth) / 2.0f;
    const float startY = 140;

    float x = startX + col * (brickWidth + horizontalGap);
    float y = startY + row * (brickHeight + verticalGap);

    bricks.push_back(Brick({x, y, brickWidth, brickHeight}, getBrickColor(row), type));
}

void Game::loadLevel(int levelNumber)
{
    bricks.clear();
    switch(levelNumber){
        case 1:
            loadLevel1();
            break;
        case 2:
            loadLevel2();
            break;
        case 3:
            loadLevel3();
            break;
        default:
            loadLevel1();
            break;
    }
}

void Game::loadLevel1()
{
    for(int row = 0; row < 4; row++){
        for(int col = 2; col < 9; col++){
            isStrongBrickLevel1(row, col) ? addStrongBrick(row, col) : addNormalBrick(row, col);
        }
    }
}

void Game::loadLevel2()
{
    for(int row = 0; row < 6; row++){
        for(int col = row; col < 11 - row; col++){
            isStrongBrickLevel2(row, col) ? addStrongBrick(row, col) : addNormalBrick(row, col);
        }
    }
}

void Game::loadLevel3()
{
    // Top row
    for(int col = 0; col < 3; col++){
        isStrongBrickLevel3(0, col) ? addStrongBrick(0, col) : addNormalBrick(0, col);
    }
    for(int col = 8; col < 11; col++){
        isStrongBrickLevel3(0, col) ? addStrongBrick(0, col) : addNormalBrick(0, col);
    }

    // Middle rows
    for(int row = 1; row <= 2; row++){
        for(int col = 0; col < 11; col++){
            isStrongBrickLevel3(row, col) ? addStrongBrick(row, col) : addNormalBrick(row, col);
        }
    }

    // Bottom row
    for(int col = 0; col < 3; col++){
        isStrongBrickLevel3(3, col) ? addStrongBrick(3, col) : addNormalBrick(3, col);
    }
    for(int col = 8; col < 11; col++){
        isStrongBrickLevel3(3, col) ? addStrongBrick(3, col) : addNormalBrick(3, col);
    }
}

void Game::resetLives()
{
    lives = MAX_LIVES;
}

void Game::resetScore()
{
    score = 0;
}

void Game::resetLevel()
{
    currentLevel = 1;
    loadLevel(currentLevel);
}

void Game::resetBricksDestroyed()
{
    bricksDestroyed = 0;
    strongBricksDestroyed = 0;
}

void Game::spawnBrickParticles(Rectangle brickBounds, Color brickColor)
{
    // Creates a small burst of particles at the center of a destroyed brick.
    // Each particle receives a random velocity so that the explosion spreads outward in different directions.
    Vector2 center = {brickBounds.x + brickBounds.width / 2.0f, brickBounds.y + brickBounds.height / 2.0f};

    for(int i = 0; i < 8; i++){
        Particle particle;

        // Spawn particle at the brick center
        particle.position = center;

        // Random velocity
        particle.velocity = {float((rand() % 201) - 100), float((rand() % 201) - 100)}; // Range: [-100, 100] pixels per second
        particle.lifetime = 0.5f;
        particle.color = Fade(brickColor, 0.8f);
        particle.size = float((rand() % 3) + 2);

        particles.push_back(particle);
    }
}

void Game::spawnArmorBreakParticles(Rectangle brickBounds, Color brickColor)
{
    // Creates a small burst of particles at the center of a destroyed brick.
    // Each particle receives a random velocity so that the explosion spreads outward in different directions.
    Vector2 center = {brickBounds.x + brickBounds.width / 2.0f, brickBounds.y + brickBounds.height / 2.0f};

    for(int i = 0; i < 4; i++){
        Particle particle;

        // Spawn particle at the brick center
        particle.position = center;

        // Random velocity
        particle.velocity = {float((rand() % 201) - 100), float((rand() % 201) - 100)}; // Range: [-100, 100] pixels per second
        particle.lifetime = 0.3f;
        particle.color = Fade(RAYWHITE, 0.8f);
        particle.size = 2.0f;

        particles.push_back(particle);
    }
}

void Game::updateParticles(float dt)
{   
    // Update position (frame-independent) and lifetime is reduced every frame.
    for(Particle& particle : particles){
        particle.position.x += particle.velocity.x * dt;
        particle.position.y += particle.velocity.y * dt;

        particle.lifetime -= dt;
    }

    // Remove particles whose lifetime has expired
    particles.erase(
        remove_if(
            particles.begin(), particles.end(), 
            [](const Particle& particle) {
                return particle.lifetime <= 0;
            }
        ), 
        particles.end());
}

void Game::drawParticles()
{
    for(const Particle& particle : particles){
        float alpha = particle.lifetime / 0.5f;
        DrawCircleV(particle.position, particle.size, Fade(particle.color, alpha));
    }
}

void Game::startScreenShake(float duration, float strength)
{
    screenShakeTime = duration;
    screenShakeStrength = strength;
}

void Game::initializeCamera()
{
    camera.offset = {0, 0};
    camera.target = {0, 0};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
}

void Game::drawOverlay(Color color)
{
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), color);
}



Color Game::getBrickColor(int row)
{
    switch(row){
        case 0:
            return {255, 100, 100, 255};   // Red
        case 1:
            return {255, 170, 80, 255};    // Orange
        case 2:
            return {255, 230, 100, 255};   // Yellow
        case 3:
            return {120, 255, 140, 255};   // Green
        case 4:
            return {80, 220, 255, 255};    // Cyan
        case 5:
            return {180, 140, 255, 255};   // Purple
        default:
            return RAYWHITE;
    }
}

void Game::restartGame()
{
    resetLives();
    resetScore();
    resetLevel();
    resetBricksDestroyed();

    paddle.reset();
    paddle.resetSize();
    paddleExpanded = false;
    paddleExpandTimer = 0.0f;

    ball.resetLaunchSpeed();
    ball.reset();
    ballLaunched = false;
    
    particles.clear();
    powerUps.clear();

    camera.offset = {0, 0};
    screenShakeTime = 0.0f;

    currentState = GameState::PLAYING;
}

void Game::handlePauseInput()
{
    if(IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_P)){
        if(currentState == GameState::PLAYING){
            currentState = GameState::PAUSED;
        }
        else if(currentState == GameState::PAUSED){
            currentState = GameState::PLAYING;
        }
    }

    if(currentState == GameState::PAUSED && IsKeyPressed(KEY_M)){
        currentState = GameState::MAIN_MENU;
    }
}

void Game::handleRestartInput()
{
    // Allow restarting after winning or losing
    if(currentState == GameState::GAME_OVER || currentState == GameState::LEVEL_COMPLETE){
        if(IsKeyPressed(KEY_ENTER)){
            restartGame();
        }
    }
    // Main-menu
    if(IsKeyPressed(KEY_M)){
        currentState = GameState::MAIN_MENU;
    }
}

void Game::handlePaddleInput(float deltaTime)
{
    // Move paddle left
    if(IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)){
        paddle.moveLeft(deltaTime);
    }

    // Move paddle right
    if(IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)){
        paddle.moveRight(deltaTime);
    }
}

void Game::handleBallLaunchInput()
{
    // Launch ball from paddle
    if(!ballLaunched && (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))){
        float paddleCenterX = paddle.getBounds().x + paddle.getBounds().width / 2.0f;
        ball.launch(paddleCenterX);
        ballLaunched = true;
    }
}

void Game::updateHighScore()
{
    if(score > highScore){
        highScore = score;
        saveHighScore();
    }
}

void Game::drawCenteredText(const char* text, int y, int fontSize, Color color)
{
    int textWidth = MeasureText(text, fontSize);
    DrawText(text, GetScreenWidth()/2 - textWidth/2, y, fontSize, color);
}

void Game::drawEndScreen(const char* title, Color titleColor, const char* instruction)
{
    drawOverlay(Fade(OVERLAY_COLOR, 0.90f));
    drawCenteredText(title, 250, 50, titleColor);
    drawCenteredText(TextFormat("Final Score: %d", score), 330, 30, SCORE_COLOR);
    drawCenteredText(TextFormat("High Score: %d", highScore), 370, 30, SCORE_COLOR);
    drawCenteredText(TextFormat("Bricks Destroyed: %d", bricksDestroyed), 410, 30, LEVEL_COLOR);
    drawCenteredText(TextFormat("Strong Bricks Destroyed: %d", strongBricksDestroyed), 450, 30, LEVEL_COLOR);
    drawCenteredText(instruction, 510, 25, INSTRUCTION_COLOR);
    drawCenteredText("M - Main Menu", 555, 25, INSTRUCTION_COLOR);
}

void Game::drawHUDText(const char* text, int centerX, Color color)
{
    int width = MeasureText(text, 30);
    DrawText(text, centerX - width / 2, 15, 30, color);
}

// void Game::drawPauseScreen()
// {
//     drawOverlay(Fade(OVERLAY_COLOR, 0.75f));
//     drawCenteredText("PAUSED", 280, 50, RAYWHITE);
//     drawCenteredText("Press P to Resume", 360, 25, INSTRUCTION_COLOR);
// }

void Game::drawPauseScreen()
{
    drawOverlay(Fade(OVERLAY_COLOR, 0.95f));
    drawCenteredText("PAUSED", 250, 55, RAYWHITE);
    drawCenteredText(TextFormat("Current Score: %d", score), 340, 30, SCORE_COLOR);
    drawCenteredText(TextFormat("High Score: %d", highScore), 380, 30, LEVEL_COLOR);
    drawCenteredText("P - Resume", 460, 25, INSTRUCTION_COLOR);
    drawCenteredText("M - Main Menu", 500, 25, INSTRUCTION_COLOR);
}

void Game::loadHighScore()
{
    ifstream file("data/highscore.txt");

    if(file.is_open()){
        file >> highScore;
        file.close();
    }
}

void Game::saveHighScore() const
{
    ofstream file("data/highscore.txt");

    if(file.is_open()){
        file << highScore;
        file.close();
    }
}

void Game::handleMainMenuInput()
{
    if(GetKeyPressed()){
        restartGame();
        currentState = GameState::PLAYING;
    }
}

void Game::drawMainMenu()
{
    drawCenteredText("BRICK BREAKER", 220, 70, SCORE_COLOR);
    drawCenteredText("Press ANY KEY to Play", 350, 30, INSTRUCTION_COLOR);
    drawCenteredText(TextFormat("High Score: %d", highScore), 420, 30, LEVEL_COLOR);
}

void Game::spawnPowerUp(Vector2 position)
{   
    // Create a falling collectible at the destroyed brick's center.
    Rectangle bounds = {position.x - 20, position.y - 10, 40, 20};
    // Randomly choose a power-up type.
    PowerUpType type = (rand() % 2 == 0) ? PowerUpType::EXPAND_PADDLE : PowerUpType::EXTRA_LIFE;
    if(lives > 4){
        type = PowerUpType::EXPAND_PADDLE;
    }
    powerUps.push_back(PowerUp(bounds, type));
}

void Game::applyPowerUp(PowerUpType type)
{   
    // Apply the collected power-up effect.
    switch(type){
        case PowerUpType::EXTRA_LIFE:
            lives++;
            break;
        case PowerUpType::EXPAND_PADDLE:
            paddle.expand();
            paddleExpanded = true;
            paddleExpandTimer += 10.0f;//stack expansion time
            paddleExpandTimer = min(paddleExpandTimer, 90.0f);// Prevent excessively long durations.
            break;
    }
}

void Game::updateExpandedPaddle(float dt)
{
    if(paddleExpanded){
        paddleExpandTimer -= dt;
        // Restore the default paddle size when the timer expires.
        if(paddleExpandTimer <= 0.0f){
            paddle.resetSize();
            paddleExpanded = false;
        }
    }
}

void Game::drawPowerUpStatus() const
{
    // Draw remaining paddle boost duration.
    if(!paddleExpanded){
        return;
    }

    const char* text = TextFormat("%.1fs", paddleExpandTimer);
    int width = MeasureText(text, 20);
    float x = paddle.getBounds().x + paddle.getBounds().width / 2.0f - width / 2.0f;
    float y = paddle.getBounds().y + paddle.getBounds().height + 15.0f;
    DrawText(text, x, y, 20, BOOST_COLOR);
    
}

void Game::updatePowerUps(float dt)
{
    // Update all active power-ups.
    for(PowerUp& powerUp : powerUps){
        powerUp.update(dt);

        // Apply effect when collected.
        if(checkPowerUpCollision(powerUp)){
            applyPowerUp(powerUp.getType());
            powerUp.collect();
        }
    }

    // Remove collected power-ups.
    powerUps.erase(
        remove_if(powerUps.begin(), powerUps.end(), [](const PowerUp& powerUp){
                return !powerUp.isActive();
            }
        ),
        powerUps.end()
    );
}



void Game::handleInput()
{   
    if(currentState == GameState::MAIN_MENU){
        handleMainMenuInput();
        return;
    }

    handlePauseInput();
    handleRestartInput();

    if(currentState != GameState::PLAYING){
        return;
    }

    float deltaTime = GetFrameTime();

    handlePaddleInput(deltaTime);
    handleBallLaunchInput();

    // Debug shortcut
    if(IsKeyPressed(KEY_N)){
        levelComplete = true;
    }
}

void Game::update()
{   
    if(currentState == GameState::PLAYING){
        float deltaTime = GetFrameTime();
        if(ballLaunched){
            ball.update(deltaTime);
        }
        else{
            float paddleCenterX = paddle.getBounds().x + paddle.getBounds().width / 2.0f;
            float paddleTopY = paddle.getBounds().y;
            ball.setPosition({paddleCenterX, paddleTopY - ball.getRadius() - 2.0f});
        }

        if(checkCollisionWithPaddle()){
            handleCollisionWithPaddle();
        }

        handleBrickCollisions();
        
        if(ball.isOutOfBounds()){
            lives--;
            if(lives <= 0){
                ball.reset();
                ballLaunched = false;
                PlaySound(gameOverSound);
                currentState = GameState::GAME_OVER;                  
            }
            else{
                ball.reset();
                paddle.reset();
                ballLaunched = false;
            }
        }
        if(isLevelComplete() || levelComplete){
            score += 500;
            updateHighScore();
            levelComplete = false;
            if(currentLevel < 3 && currentLevel >= 1){
                PlaySound(levelCompleteSound);            
            }

            if(currentLevel < 3){
                // Increase level counter, load next level, reset entities 
                currentLevel++;
                loadLevel(currentLevel);

                ball.increaseLaunchSpeed(30.0f);
                resetLives();
                paddle.reset();
                ball.reset();
                ballLaunched = false;
            }
            else{
                currentState = GameState::LEVEL_COMPLETE;
                PlaySound(victorySound);
            }
        }
        if(screenShakeTime > 0){
            screenShakeTime -= deltaTime;
            camera.offset.x = float((rand() % int(screenShakeStrength * 2 + 1)) - screenShakeStrength);
            camera.offset.y = float((rand() % int(screenShakeStrength * 2 + 1)) - screenShakeStrength);
        }
        else{
            camera.offset = {0, 0};
        }
        updateParticles(deltaTime);
        updatePowerUps(deltaTime);
        updateExpandedPaddle(deltaTime);
    }
}

void Game::draw()
{   
    // Render Main-Menu
    if(currentState == GameState::MAIN_MENU){
        drawMainMenu();
        return; 
    }

    BeginMode2D(camera);
    
    // Render entities
    paddle.drawPaddle();
    ball.drawBall();
    for(const Brick& brick : bricks){
        brick.draw();
    }
    for(const PowerUp& powerUp : powerUps){
        powerUp.draw();
    }

    drawParticles();
    drawPowerUpStatus();

    EndMode2D();

    // Display Game Status
    DrawRectangle(0, 0, GetScreenWidth(), 60, Fade(RAYWHITE, 0.05f));
    drawHUDText(TextFormat("Lives: %d", lives), 160, LIVES_COLOR);
    drawHUDText(TextFormat("Level: %d", currentLevel), 480, LEVEL_COLOR);
    drawHUDText(TextFormat("Best: %d", highScore), 800, SCORE_COLOR);
    drawHUDText(TextFormat("Score: %d", score), 1120, SCORE_COLOR);
    DrawText("P - Pause", GetScreenWidth() - 140, GetScreenHeight() - 30, 20, Fade(INSTRUCTION_COLOR, 0.6f));

    float textY = paddle.getBounds().y - 75.0f;
    if(!ballLaunched && currentState == GameState::PLAYING){
        DrawText("Press UP to Launch", GetScreenWidth()/2 - 110, textY, 25, INSTRUCTION_COLOR);
    }

    // Draw level-complete overlay
    if(currentState == GameState::LEVEL_COMPLETE){
        drawEndScreen("YOU WIN!", LEVEL_COMPLETE_COLOR, "ENTER - Play again");
    }

    //Draw game-over overlay
    if(currentState == GameState::GAME_OVER){
        drawEndScreen("GAME OVER", GAME_OVER_COLOR, "ENTER - Play again");
    }

    //Draw PAUSE overlay
    if(currentState == GameState::PAUSED){
        drawPauseScreen();
    }
}

Game::~Game()
{
    UnloadSound(paddleHitSound);
    UnloadSound(brickBreakSound);
    UnloadSound(levelCompleteSound);
    UnloadSound(gameOverSound);
    UnloadSound(armorBreakSound);
    UnloadSound(victorySound);
}
