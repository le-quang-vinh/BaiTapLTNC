// init.h
#ifndef INIT_H
#define INIT_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

bool initSDL();                // Khởi tạo SDL, window và renderer
SDL_Texture* loadTexture(const char* path); // Tải ảnh
void initAudio();              // Khởi tạo âm thanh

#endif

