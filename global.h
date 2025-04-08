// global.h
#ifndef GLOBAL_H
#define GLOBAL_H
#include <vector>
#include <SDL.h>
#include <SDL_mixer.h>
#include "global.h"
#include "render.h"
extern SDL_Window* window;
extern SDL_Renderer* renderer;

extern SDL_Texture* playerIdleTexture;
extern SDL_Texture* playerTexture;
extern SDL_Texture* backgroundTexture;
extern SDL_Texture* enemyTexture;
extern SDL_Texture* gameOverTexture;
extern SDL_Texture* restartButtonTexture;
extern SDL_Texture* exitButtonTexture;
extern SDL_Texture* youWinTexture;
extern SDL_Texture* auraTextures[4];
extern SDL_Texture* chaoTexture;

extern Mix_Music* bgm;
extern Mix_Chunk* chargeSound;
extern Mix_Chunk* chaoSound;

extern SDL_Rect enemyRect;
extern SDL_Rect restartButton;
extern SDL_Rect exitButton;
extern SDL_Rect playerRect;

extern bool movingRight;
extern bool isGameOver;
extern bool isJumping;
extern bool isStopped;
extern bool canShootChao;
extern bool gameWon;
extern bool isCharging;
extern double jumpVelocity;
extern std::vector<Enemy> enemies;
extern std::vector<Chao> chaos;
extern int energy;
extern int chaoCooldown;
extern int auraTimer;
extern double jumpFrame;
#endif

