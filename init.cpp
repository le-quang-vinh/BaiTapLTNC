
#include "init.h"
#include <iostream>
#include "global.h"
#include "config.h"
// Bạn cần include biến window, renderer, bgm, chargeSound, chaoSound từ nơi khai báo
extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern Mix_Music* bgm;
extern Mix_Chunk* chargeSound;
extern Mix_Chunk* chaoSound;

bool initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    window = SDL_CreateWindow("Simple SDL2 Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    return true;
}

SDL_Texture* loadTexture(const char* path) {
    SDL_Surface* loadedSurface = IMG_Load(path);
    if (!loadedSurface) {
        std::cout << "Unable to load image " << path << "! SDL_image Error: " << IMG_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);
    return texture;
}

void initAudio() {
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    bgm = Mix_LoadMUS("C:\\Teach1\\LTNC\\2024\\test4\\Data\\Music\\backgroundmusic.mp3");
    if (bgm) {
        Mix_PlayMusic(bgm, -1);
    }

    chargeSound = Mix_LoadWAV("C:\\Teach1\\LTNC\\2024\\test4\\Data\\Music\\chargeSound.wav");
    if (!chargeSound) {
        std::cout << "Failed to load charge sound! " << Mix_GetError() << std::endl;
    }

    chaoSound = Mix_LoadWAV("C:\\Teach1\\LTNC\\2024\\test4\\Data\\Music\\chaoSound.wav");
    if (!chaoSound) {
        std::cout << "Failed to load chao sound! " << Mix_GetError() << std::endl;
    }
}

