#include<raylib.h>
#include "game.hpp"

// Global Constants
const int FPS = 120;
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
// const Color backgroundColor = { 25, 30, 45, 255 };
const Color backgroundColor = { 8, 8, 8, 255 };

int main(){
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Brick Breaker");
    InitAudioDevice();
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
    CloseAudioDevice();
    return 0;
}