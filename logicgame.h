#pragma once
#include <SDL.h>
void resetGame();
void updateEnemies();
void shootChao();
void updateCooldown();
void updateChaos();
void updateEnergy();
void updateJump();
bool checkCollision(SDL_Rect a, SDL_Rect b);
