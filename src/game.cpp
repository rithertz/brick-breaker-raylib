#include "game.hpp"

Game::Game() : 
    paddle(Rectangle{580, 640, 120, 25}), 
    ball({640, 600}, {300, 300})
{
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
}

void Game::draw()
{
    paddle.drawPaddle();
    ball.drawBall();
}
