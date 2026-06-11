#include<raylib.h>
#include "game.hpp"


// Global Constants
const int FPS = 60;
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const Color backgroundColor = { 25, 30, 45, 255 };
// Variable constants

int main(){
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Brick Breaker");
    SetTargetFPS(FPS);
    Game game;
    // Game Loop
    while(!WindowShouldClose()){
        // 1. Event Handling
        game.handleInput();
        // 2. Updating positions
        game.update();
        // 3. Drawing
        BeginDrawing();
        ClearBackground(backgroundColor);
        game.draw();
        EndDrawing();
    }
    CloseWindow(); 
    return 0;
}