#include "game.h"
#include "render.h"
#include "config.h"
#include "input.h"
#include "global.h"
#include "init.h"
#include "logicgame.h"
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

SDL_Texture* playerIdleTexture = nullptr;
SDL_Texture* playerTexture = nullptr;
SDL_Texture* backgroundTexture = nullptr;
SDL_Texture* enemyTexture = nullptr;
SDL_Texture* gameOverTexture = nullptr;
SDL_Texture* restartButtonTexture = nullptr;
SDL_Texture* exitButtonTexture = nullptr;
SDL_Texture* youWinTexture = nullptr;
SDL_Texture* auraTextures[4];
SDL_Texture* chaoTexture = nullptr;

Mix_Music* bgm = nullptr;
Mix_Chunk* chargeSound = nullptr;
Mix_Chunk* chaoSound = nullptr;

SDL_Rect enemyRect = {200, 500, 50, 50};
SDL_Rect restartButton = {100, 450, 200, 50};
SDL_Rect exitButton = {500, 450, 200, 50};
SDL_Rect playerRect = {100, 500, 50, 50};

int auraFrame = 0;
int auraTimer = 0;
int energy = 0;
int chaoCooldown = 0;
double jumpVelocity = -10.0;
double jumpFrame = 0;
double playerY = groundY;

bool movingRight = true;
bool isGameOver = false;
bool isJumping = false;
bool isStopped = false;
bool canShootChao = false;
bool gameWon = false;
bool isCharging = false;

std::vector<Enemy> enemies;
std::vector<Chao> chaos;


void cleanUp() {
    SDL_DestroyTexture(playerIdleTexture);
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(gameOverTexture);
    SDL_DestroyTexture(restartButtonTexture);
    SDL_DestroyTexture(exitButtonTexture);
    SDL_DestroyTexture(youWinTexture);
    Mix_FreeMusic(bgm);

    Mix_FreeChunk(chargeSound);
    Mix_FreeChunk(chaoSound);
    Mix_CloseAudio();
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
    backgroundTexture = loadTexture("C:\\Teach1\\LTNC\\2024\\test4\\Data\\Image\\background.png");
    if (!backgroundTexture) return -1;
    // Tải ảnh nhân vật bình thường
    playerIdleTexture = loadTexture("C:\\Teach1\\LTNC\\2024\\test4\\Data\\Image\\player.png");
    if (!playerIdleTexture) return -1;
    playerTexture = playerIdleTexture;  // Mặc định là trạng thái đứng yên
 enemyTexture = loadTexture("C:\\Teach1\\LTNC\\2024\\test4\\Data\\Image\\enemy.png");
 gameOverTexture = loadTexture("C:\\Teach1\\LTNC\\2024\\test4\\Data\\Image\\gameover.png");
 restartButtonTexture = loadTexture("C:\\Teach1\\LTNC\\2024\\test4\\Data\\Image\\restart.png"); // Thay thế bằng file của bạn
exitButtonTexture = loadTexture("C:\\Teach1\\LTNC\\2024\\test4\\Data\\Image\\exit.png");
auraTextures[0] = loadTexture("C:\\Teach1\\LTNC\\2024\\test4\\Data\\Image\\aura1.png");
auraTextures[1] = loadTexture("C:\\Teach1\\LTNC\\2024\\test4\\Data\\Image\\aura2.png");
auraTextures[2] = loadTexture("C:\\Teach1\\LTNC\\2024\\test4\\Data\\Image\\aura3.png");
auraTextures[3] = loadTexture("C:\\Teach1\\LTNC\\2024\\test4\\Data\\Image\\aura4.png");
if (!restartButtonTexture || !exitButtonTexture) {
    std::cout << "Failed to load button textures!\n";
    return -1;
}
chaoTexture = loadTexture("C:\\Teach1\\LTNC\\2024\\test4\\Data\\Image\\ki.png"); // Thay "chao.png" bằng đường dẫn đến ảnh chưởng của bạn
if (!chaoTexture) {
    std::cout << "Failed to load chao texture!\n";
    return -1;
}
youWinTexture = loadTexture("C:\\Teach1\\LTNC\\2024\\test4\\Data\\Image\\youwin.png");
if (!gameOverTexture) return -1;
if (!enemyTexture) return -1;
    bool running = true;
    bool canShootChao = true;
    initAudio();
    enemies.push_back({{200, 500, 50, 50}, 3, true}); // Thêm kẻ địch khởi tạo
   while (running) {
    handleEvents(running);
    if(!isStopped){
    handleInput(running);
    updateJump();
    updateEnemies();  // Cập nhật vị trí quái vật
    updateEnergy();
    updateChaos();
    updateCooldown();
 for(size_t i=0;i<enemies.size();i++){
    if (checkCollision(playerRect, enemies[i].rect)) {
    isStopped = true;
    isGameOver =true;
    break;
}
    }
     if (energy >= CHAO_COST && chaoCooldown <= 0) {
            if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_RETURN]) {  // Kiểm tra phím Enter
                shootChao();  // Gọi hàm shootChao để bắn chưởng
            }
        }
    }
    render();
    SDL_Delay(16);
}


    cleanUp();
    return 0;
}
