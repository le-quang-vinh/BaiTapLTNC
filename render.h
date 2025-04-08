#ifndef RENDER_H
#define RENDER_H

#include <SDL.h>
#include "game.h"
#include "config.h"
struct Enemy {
    SDL_Rect rect;
    int speed;
    bool movingRight;
};

struct Chao {
    SDL_Rect rect;
    int speed;
};
extern SDL_Renderer* renderer;
extern SDL_Texture* backgroundTexture;
extern SDL_Texture* playerTexture;
extern SDL_Texture* enemyTexture;
extern SDL_Texture* chaoTexture;
extern SDL_Texture* auraTextures[];
extern SDL_Texture* gameOverTexture;
extern SDL_Texture* youWinTexture;
extern SDL_Texture* restartButtonTexture;
extern SDL_Texture* exitButtonTexture;

extern SDL_Rect playerRect;
extern SDL_Rect restartButton;
extern SDL_Rect exitButton;

extern std::vector<Enemy> enemies;
extern std::vector<Chao> chaos;

extern int auraFrame;
extern bool isCharging;
extern bool isGameOver;
extern bool gameWon;
extern int energy;


// Hàm vẽ
void render();
void renderEnergyBar();

#endif
