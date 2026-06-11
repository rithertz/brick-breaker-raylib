# Brick Breaker

A Brick Breaker clone built completely from scratch in C++ using Raylib.

## Tech Stack

* C++17
* Raylib
* Git
* GitHub

## Goals

* Implement game architecture
* Implement object-oriented design
* Build reusable game systems

## Current Progress

### Milestone 0 - Project Setup

* Git repository created
* GitHub repository created
* Raylib configured
* Window creation implemented
* 1280x720 game window
* 60 FPS game loop

### Milestone 1 — Paddle

Completed:

* Paddle class created
* Rounded paddle rendering implemented 
* Frame-independent paddle movement implemented using delta time
* Screen boundary clamping implemented

### Milestone 2 - Game Architecture & Ball

Completed:

* Game class created
* Input handling moved into Game
* Ball class created
* Ball rendering implemented
* Frame-independent ball movement implemented
* Wall collision detection implemented
* Ball reflection on all four screen boundaries implemented
* Game class now composes and manages all gameplay entities:
  - Paddle (ownership, input handling, update/draw delegation)
  - Ball (ownership, frame-independent movement, collision detection)

## Screenshots

### Milestone 1 - Paddle

![Milestone 1 Paddle](screenshots/milestone-1-paddle.png)

### Milestone 2 - Ball


![Milestone 2 Ball](screenshots/milestone-2-ball.png)


## Planned Features

### Core Gameplay

* Paddle-Ball collision
* Bricks
* Brick destruction
* Multiple levels

### Game Systems

* Score system
* Lives system
* Multiple levels
* Win/Lose screens

### Polish

* Sound effects
* Particle effects
* Power-ups

### Stretch Goals

* Level editor
* Additional power-up types
* Level loading from files

## Repository Structure

```text
src/ 
    ├── main.cpp 
    ├── game.hpp 
    ├── game.cpp 
    ├── paddle.hpp 
    ├── paddle.cpp 
    ├── ball.hpp 
    └── ball.cpp
```
