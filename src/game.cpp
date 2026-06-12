#include "game.hpp"
#include "raymath.h"

Game::Game() : paddle(Rectangle{580, 640, 120, 25}),
               ball({640, 630}, {300, 300})
{
}

bool Game::checkCollisionWithPaddle()
{
    // Checks for collision && if ball is moving down && ball is above paddle
    float ballBottom = ball.getPosition().y + ball.getRadius();
    float paddleBottom = paddle.getBounds().y + paddle.getBounds().height;

    return CheckCollisionCircleRec(ball.getPosition(), ball.getRadius(), paddle.getBounds()) 
        && ball.isMovingDown() 
        && ballBottom < paddleBottom;
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

void Game::handleInput()
{   
    float deltaTime = GetFrameTime();
    if(IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)){
        paddle.moveLeft(deltaTime);
    }
    if(IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)){
        paddle.moveRight(deltaTime);
    }
}

void Game::update()
{   
    float deltaTime = GetFrameTime();
    ball.update(deltaTime);

    if(checkCollisionWithPaddle()){
        handleCollisionWithPaddle();
    }
}

void Game::draw()
{
    paddle.drawPaddle();
    ball.drawBall();
}
