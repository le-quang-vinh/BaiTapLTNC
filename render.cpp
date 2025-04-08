#include "render.h"
#include "game.h"
#include "config.h"
#include "global.h"
void renderEnergyBar() {
    if (isGameOver || gameWon) {
        return;
    }

    SDL_Rect energyBarOutline = {50, 50, 200, 20}; // Viền thanh năng lượng
    SDL_Rect energyBarFill = {50, 50, (energy * 200) / maxEnergy, 20}; // Phần năng lượng

    // Vẽ viền (màu trắng)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &energyBarOutline);

    // Vẽ thanh năng lượng (màu xanh lá)
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &energyBarFill);
}
void render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    if (!gameWon) {
        SDL_Rect energyBar = {50, 50, energy * 2, 20};
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &energyBar);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    }

    if (gameWon && youWinTexture) {
        SDL_Rect destRect = {150, 100, 500, 300};
        SDL_RenderCopy(renderer, youWinTexture, NULL, &destRect);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        if (restartButtonTexture) {
            SDL_RenderCopy(renderer, restartButtonTexture, NULL, &restartButton);
        }
        if (exitButtonTexture) {
            SDL_RenderCopy(renderer, exitButtonTexture, NULL, &exitButton);
        }
    } else if (isGameOver) {
        SDL_RenderCopy(renderer, gameOverTexture, NULL, NULL);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        if (restartButtonTexture) {
            SDL_RenderCopy(renderer, restartButtonTexture, NULL, &restartButton);
        }
        if (exitButtonTexture) {
            SDL_RenderCopy(renderer, exitButtonTexture, NULL, &exitButton);
        }
    } else {
        if (backgroundTexture) {
            SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
        }

        if (playerTexture) {
            SDL_RenderCopy(renderer, playerTexture, NULL, &playerRect);
        }

        for (const Enemy& enemy : enemies) {
            SDL_RenderCopy(renderer, enemyTexture, NULL, &enemy.rect);
        }

        for (const Chao& chao : chaos) {
            SDL_RenderCopy(renderer, chaoTexture, NULL, &chao.rect);
        }

        if (isCharging) {
            SDL_Rect auraRect = {playerRect.x - 10, playerRect.y - 10, playerRect.w + 20, playerRect.h + 20};
            SDL_RenderCopy(renderer, auraTextures[auraFrame], NULL, &auraRect);
        }

        renderEnergyBar();
    }

    SDL_RenderPresent(renderer);
}
