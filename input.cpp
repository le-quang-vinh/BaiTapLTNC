#include "input.h"
#include "config.h"
#include "game.h"
#include <SDL.h>
extern bool isGameOver;
extern bool gameWon;
extern bool isCharging;
extern bool isStopped;
extern bool canShootChao;
extern bool isJumping;
extern int energy;
extern const int PLAYER_SPEED;
extern const int CHAO_COST;

extern double jumpFrame;
extern SDL_Rect playerRect;
extern SDL_Rect restartButton;
extern SDL_Rect exitButton;

// "extern" các hàm định nghĩa nơi khác
extern void resetGame();
extern void shootChao();
void handleEvents(bool& running) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            running = false;
        }

        if (e.type == SDL_MOUSEBUTTONDOWN && (isGameOver || gameWon)) {
            int mouseX = e.button.x;
            int mouseY = e.button.y;

            if (mouseX > restartButton.x && mouseX < restartButton.x + restartButton.w &&
                mouseY > restartButton.y && mouseY < restartButton.y + restartButton.h) {
                resetGame(); // Khởi động lại game
            }

            if (mouseX > exitButton.x && mouseX < exitButton.x + exitButton.w &&
                mouseY > exitButton.y && mouseY < exitButton.y + exitButton.h) {
                SDL_Quit();
                exit(0);
            }
        }
    }
}

void handleInput(bool& running) {
    if (isStopped) return;

    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    if (keystate[SDL_SCANCODE_UP] && !isJumping) {
        isJumping = true;
        jumpFrame = 0;
    }
    if (!isJumping) {
        if (keystate[SDL_SCANCODE_DOWN]) playerRect.y += PLAYER_SPEED;
        if (keystate[SDL_SCANCODE_LEFT]) playerRect.x -= PLAYER_SPEED;
        if (keystate[SDL_SCANCODE_RIGHT]) playerRect.x += PLAYER_SPEED;
    }

    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            running = false;
        }

        if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_UP && !isJumping) {
                isJumping = true;
                jumpFrame = 0;
            }
            if (e.key.keysym.sym == SDLK_DOWN && !isJumping) {
                playerRect.y += PLAYER_SPEED;
            }
            if (e.key.keysym.sym == SDLK_LEFT && !isJumping) {
                playerRect.x -= PLAYER_SPEED;
            }
            if (e.key.keysym.sym == SDLK_RIGHT && !isJumping) {
                playerRect.x += PLAYER_SPEED;
            }
            if (e.key.keysym.sym == SDLK_SPACE) {
                isCharging = true;
            }
            if (e.key.keysym.sym == SDLK_RETURN) {
                if (canShootChao && energy >= CHAO_COST) {
                    shootChao();
                    energy -= CHAO_COST;
                    canShootChao = false;
                }
            }
        } else if (e.type == SDL_KEYUP) {
            if (e.key.keysym.sym == SDLK_SPACE) {
                isCharging = false;
            }
        }
    }

    if (isCharging && energy < maxEnergy) {
        energy += 1;
    }
}

