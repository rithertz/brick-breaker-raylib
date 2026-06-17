#include "game.hpp"
#include "raymath.h"
#include <iostream>
using namespace std;

const Color SCORE_COLOR          = {255, 215, 100, 255};
const Color LIVES_COLOR          = {255, 120, 120, 255};
const Color LEVEL_COLOR          = {80, 220, 255, 255};
const Color GAME_OVER_COLOR      = {255, 80, 80, 255};
const Color LEVEL_COMPLETE_COLOR = {120, 255, 140, 255};
const Color INSTRUCTION_COLOR = {200, 210, 230, 255};
const int MAX_LIVES = 3;


Game::Game() : paddle(Rectangle{580, 660, 150, 25}),
               ball({640, 630}, {900, 900}),               
               lives(MAX_LIVES),
               ballLaunched(false),
               currentLevel(1),
               levelComplete(false),
               score(0),
               currentState(GameState::PLAYING)
{
    loadLevel(1);
    //initializeBricks();
}

bool Game::checkCollisionWithPaddle()
{
    // Check for collision && if ball is moving down && ball is above paddle
    return CheckCollisionCircleRec(ball.getPosition(),ball.getRadius(),paddle.getBounds())
        && ball.isMovingDown() 
        && ball.getPreviousPosition().y < paddle.getBounds().y;
}

void Game::handleCollisionWithPaddle()
{   
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
    for(Brick& brick : bricks)
    {
        if(brick.isAlive() && CheckCollisionCircleRec(ball.getPosition(), ball.getRadius(), brick.getBounds()))
        {
            Rectangle bounds = brick.getBounds();

            Vector2 prevPos = ball.getPreviousPosition();
            float radius = ball.getRadius();

            float left   = bounds.x;
            float right  = bounds.x + bounds.width;

            ball.revertToPreviousPosition();

            if(prevPos.x + radius <= left)
            {
                // Hit left side
                ball.invertXVelocity();
            }
            else if(prevPos.x - radius >= right)
            {
                // Hit right side
                ball.invertXVelocity();
            }
            else
            {
                // Hit top or bottom
                ball.invertYVelocity();
            }

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
    const float brickWidth = 90;
    const float brickHeight = 25;
    const float horizontalGap = 10;
    const float verticalGap = 10;

    const int columns = 11;

    float totalWidth =
        columns * brickWidth +
        (columns - 1) * horizontalGap;

    float startX =
        (GetScreenWidth() - totalWidth) / 2.0f;

    const float startY = 120;

    float x = startX + col * (brickWidth + horizontalGap);
    float y = startY + row * (brickHeight + verticalGap);

    bricks.push_back(
        Brick({x, y, brickWidth, brickHeight})
    );
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
    for(int row = 0; row < 6; row++)
    {
        for(int col = row; col < 11 - row; col++)
        {
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
    if(currentState == GameState::GAME_OVER)
    {
        if(IsKeyPressed(KEY_ENTER))
        {
            restartGame();
        }
        return;
    }

    if(currentState == GameState::LEVEL_COMPLETE)
    {
        if(IsKeyPressed(KEY_ENTER))
        {
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
    if(!ballLaunched && (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)))
    {   
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
    }
}

void Game::draw()
{   

    paddle.drawPaddle();
    ball.drawBall();
    for(const Brick& brick : bricks){
        brick.draw();
    }
    // if(currentState == GameState::LEVEL_COMPLETE){
    //     DrawText("LEVEL COMPLETE!", GetScreenWidth()/2 - 200, GetScreenHeight()/2 - 20, 40, LEVEL_COMPLETE_COLOR);
    // }
    // if(currentState == GameState::GAME_OVER){
    //     DrawText("GAME OVER", GetScreenWidth()/2 - 150, GetScreenHeight()/2 - 20, 50, GAME_OVER_COLOR);
    // }
    DrawText(TextFormat("Lives: %d", lives),20, 20, 30, LIVES_COLOR);
    DrawText(TextFormat("Level: %d", currentLevel), 20, 55, 30, LEVEL_COLOR);
    DrawText(TextFormat("Score: %d", score), 20, 90, 30, SCORE_COLOR);
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

