#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int PLAYER_SPEED = 5;
const int JUMP_FRAMES = 60;
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* playerIdleTexture = nullptr;  // Ảnh nhân vật khi đứng yên
SDL_Texture* playerTexture = nullptr;      // Ảnh hiện tại của nhân vật
SDL_Texture* backgroundTexture = nullptr;  // Ảnh nền
SDL_Texture* enemyTexture = nullptr;  // Thêm dòng này ở đầu file, trước main()
SDL_Rect enemyRect = {200, 500, 50, 50};
int enemySpeed = 3;
bool movingRight = true;


SDL_Rect playerRect = {100, 500, 50, 50};

bool isJumping = false;
double jumpFrame = 0;
double jumpVelocity = -10; // Tốc độ nhảy lên
double gravity = 0.5;        // Tốc độ rơi xuống
double groundY = 500;      // Vị trí mặt đất của nhân vật (điều chỉnh theo ý bạn)
double playerY = groundY;
bool initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
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

void handleEvents(bool& running) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            running = false;
        }
    }
}
void updateEnemy() {

    if (movingRight) {
        enemyRect.x += enemySpeed;
        if (enemyRect.x + enemyRect.w >= SCREEN_WIDTH) {
            movingRight = false;  // Chạm cạnh phải → Đổi hướng
        }
    } else {
        enemyRect.x -= enemySpeed;
        if (enemyRect.x <= 0) {
            movingRight = true;  // Chạm cạnh trái → Đổi hướng
        }
    }
}

void handleInput() {
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
}

void updateJump() {
    if (isJumping) {
        playerRect.y += jumpVelocity;  // Cập nhật vị trí nhân vật
        jumpVelocity += gravity;       // Áp dụng trọng lực

        // Kiểm tra nếu chạm đất
        if (playerRect.y >= groundY) {
            playerRect.y = groundY;   // Cố định vị trí trên mặt đất
            isJumping = false;        // Dừng nhảy
            jumpFrame = 0;            // Reset khung hình nhảy
            jumpVelocity = -10;       // Reset vận tốc cho lần nhảy sau
        }
    }
}


void render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    if (backgroundTexture) {
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
    }

    if (playerTexture) {
        SDL_RenderCopy(renderer, playerTexture, NULL, &playerRect);
    }
     if (enemyTexture) {
        SDL_RenderCopy(renderer, enemyTexture, NULL, &enemyRect);
    }
    SDL_RenderPresent(renderer);
}

void cleanUp() {
    SDL_DestroyTexture(playerIdleTexture);
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    if (!initSDL()) return -1;
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
        return -1;
    }

    // Tải ảnh nền
    backgroundTexture = loadTexture("background.png");
    if (!backgroundTexture) return -1;

    // Tải ảnh nhân vật bình thường
    playerIdleTexture = loadTexture("player.png");
    if (!playerIdleTexture) return -1;
    playerTexture = playerIdleTexture;  // Mặc định là trạng thái đứng yên

 enemyTexture = loadTexture("enemy.png");
if (!enemyTexture) return -1;
    bool running = true;
   while (running) {
    handleEvents(running);
    handleInput();
    updateJump();
    updateEnemy();  // Cập nhật vị trí quái vật
    render();
    SDL_Delay(16);
}


    cleanUp();
    return 0;
}
