#include "game.hpp"
#include "raymath.h"
#include <iostream>
#include <cstdlib>
#include <algorithm>

using namespace std;

// Game Constants
const Color SCORE_COLOR = {255, 215, 100, 255};
const Color LIVES_COLOR = {255, 120, 120, 255};
const Color LEVEL_COLOR = {80, 220, 255, 255};
const Color GAME_OVER_COLOR = {255, 80, 80, 255};
const Color LEVEL_COMPLETE_COLOR = {120, 255, 140, 255};
const Color INSTRUCTION_COLOR = {200, 210, 230, 255};
const int MAX_LIVES = 3;


Game::Game() : lives(MAX_LIVES), currentLevel(1), score(0),
               ballLaunched(false), levelComplete(false),
               paddle(Rectangle{580, 660, 150, 25}), ball({640, 630}, {900, 900}),
               currentState(GameState::PLAYING)
{   
    initSounds();
    loadLevel(1);    
}

void Game::initSounds()
{   
    // Load Sounds
    paddleHitSound = LoadSound("assets/sounds/paddle_hit.wav");
    brickBreakSound = LoadSound("assets/sounds/brick_break.wav");
    levelCompleteSound = LoadSound("assets/sounds/level_complete.wav"); 
    gameOverSound = LoadSound("assets/sounds/game_over.wav");

    // Adjust Volume
    SetSoundVolume(brickBreakSound, 0.5f);
    SetSoundVolume(paddleHitSound, 0.7f);
    SetSoundVolume(levelCompleteSound, 1.0f);
    SetSoundVolume(gameOverSound, 1.0f);
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

            spawnBrickParticles(brick.getBounds(), brick.getColor());

            PlaySound(brickBreakSound);
            brick.destroy();
            score += 100;
            break;
        }
    }
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

void Game::addBrick(int row, int col)
{
    const int columns = 11;

    const float brickWidth = 90;
    const float brickHeight = 25;
    const float horizontalGap = 10;
    const float verticalGap = 10;

    float totalWidth = columns * brickWidth + (columns - 1) * horizontalGap;
    
    float startX = (GetScreenWidth() - totalWidth) / 2.0f;
    const float startY = 120;

    float x = startX + col * (brickWidth + horizontalGap);
    float y = startY + row * (brickHeight + verticalGap);

    bricks.push_back(Brick({x, y, brickWidth, brickHeight}, getBrickColor(row)));
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
            addBrick(row, col);
        }
    }
}

void Game::loadLevel2()
{
    for(int row = 0; row < 6; row++){
        for(int col = row; col < 11 - row; col++){
            addBrick(row, col);
        }
    }
}

void Game::loadLevel3()
{
    // Top row
    for(int col = 0; col < 3; col++){
        addBrick(0, col);
    }
    for(int col = 8; col < 11; col++){
        addBrick(0, col);
    }

    // Middle rows
    for(int row = 1; row <= 2; row++){
        for(int col = 0; col < 11; col++){
            addBrick(row, col);
        }
    }

    // Bottom row
    for(int col = 0; col < 3; col++){
        addBrick(3, col);
    }
    for(int col = 8; col < 11; col++){
        addBrick(3, col);
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

    paddle.reset();

    ball.resetLaunchSpeed();
    ball.reset();
    ballLaunched = false;

    currentState = GameState::PLAYING;
}
void Game::handleInput()
{   
    if(currentState == GameState::GAME_OVER){
        if(IsKeyPressed(KEY_ENTER)){
            restartGame();
        }
        return;
    }
    if(currentState == GameState::LEVEL_COMPLETE){
        if(IsKeyPressed(KEY_ENTER)){
            restartGame();
        }
        return;
    }

    float deltaTime = GetFrameTime();

    if(IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)){
        paddle.moveLeft(deltaTime);
    }
    if(IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)){
        paddle.moveRight(deltaTime);
    }
    if(!ballLaunched && (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))){   
        float paddleCenterX = paddle.getBounds().x + paddle.getBounds().width/2;
        ball.launch(paddleCenterX);
        ballLaunched = true;
    }
    //for testing
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
            ball.setPosition({paddleCenterX, paddleTopY - ball.getRadius()});
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
            levelComplete = false;
            PlaySound(levelCompleteSound);            

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
            }
        }
        updateParticles(deltaTime);
    }
}

void Game::draw()
{   
    // Render entities
    paddle.drawPaddle();
    ball.drawBall();
    for(const Brick& brick : bricks){
        brick.draw();
    }

    drawParticles();

    // Display Game Status 
    DrawText(TextFormat("Lives: %d", lives), 20, 20, 30, LIVES_COLOR);
    DrawText(TextFormat("Level: %d", currentLevel), 20, 55, 30, LEVEL_COLOR);
    DrawText(TextFormat("Score: %d", score), 20, 90, 30, SCORE_COLOR);

    // Draw game over or level complete overlays
    if(currentState == GameState::LEVEL_COMPLETE){
        DrawText("YOU WIN!", GetScreenWidth()/2 - 120, GetScreenHeight()/2 - 50, 50, LEVEL_COMPLETE_COLOR);
        DrawText("Press ENTER to play again", GetScreenWidth()/2 - 170, GetScreenHeight()/2 + 20, 25, INSTRUCTION_COLOR);
        DrawText(TextFormat("Final Score: %d", score), GetScreenWidth()/2 - 120, GetScreenHeight()/2 + 60, 25, SCORE_COLOR);
    }
    if(currentState == GameState::GAME_OVER){
        DrawText("GAME OVER", GetScreenWidth()/2 - 150, GetScreenHeight()/2 - 50, 50, GAME_OVER_COLOR);
        DrawText("Press ENTER to restart", GetScreenWidth()/2 - 150, GetScreenHeight()/2 + 20, 25, INSTRUCTION_COLOR);
        DrawText(TextFormat("Final Score: %d", score), GetScreenWidth()/2 - 120, GetScreenHeight()/2 + 60, 25, SCORE_COLOR);
    }
}

Game::~Game()
{
    UnloadSound(paddleHitSound);
    UnloadSound(brickBreakSound);
    UnloadSound(levelCompleteSound);
    UnloadSound(gameOverSound);
}
