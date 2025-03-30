#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int PLAYER_SPEED = 5;
const int JUMP_FRAMES = 60;
const int MAX_ENEMIES =10 ;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* playerIdleTexture = nullptr;  // Ảnh nhân vật khi đứng yên
SDL_Texture* playerTexture = nullptr;      // Ảnh hiện tại của nhân vật
SDL_Texture* backgroundTexture = nullptr;  // Ảnh nền
SDL_Texture* enemyTexture = nullptr;  // Thêm dòng này ở đầu file, trước main()
SDL_Rect enemyRect = {200, 500, 50, 50};
SDL_Rect restartButton = {100, 450, 200, 50}; // Nút Restart
SDL_Rect exitButton = {500, 450, 200, 50};    // Nút Exit
SDL_Texture* gameOverTexture = nullptr; // Thêm texture cho màn hình game over
SDL_Texture* restartButtonTexture = nullptr;
SDL_Texture* exitButtonTexture = nullptr;

SDL_Rect playerRect = {100, 500, 50, 50};
int enemySpeed = 3;
bool movingRight = true;
bool isGameOver = false;
bool isJumping = false;
bool isStopped = false;

int energy = 0;        // Giá trị năng lượng hiện tại
const int maxEnergy = 100; // Giới hạn tối đa của năng lượng
bool isCharging = false; // Kiểm tra xem có đang tích lũy năng lượng không
double jumpFrame = 0;
double jumpVelocity = -10; // Tốc độ nhảy lên
double gravity = 0.5;        // Tốc độ rơi xuống
double groundY = 500;      // Vị trí mặt đất của nhân vật (điều chỉnh theo ý bạn)
double playerY = groundY;
struct Enemy {
     SDL_Rect rect;
     int speed;
     bool movingRight;};

     std::vector<Enemy> enemies;
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
bool checkCollision(SDL_Rect a, SDL_Rect b) {
    return (a.x +10 < b.x + b.w -10 &&
            a.x + a.w -10> b.x +10 &&
            a.y < b.y + b.h -5 &&
            a.y + a.h > b.y +5);
}
void resetGame() {
    // Đặt lại vị trí nhân vật
    playerRect.x = 100;
    playerRect.y = groundY;

    // Reset trạng thái
    isGameOver = false;
    isStopped = false;
    isJumping = false;
    jumpVelocity = -10; // Đặt lại vận tốc nhảy

    // Xóa toàn bộ danh sách kẻ địch
    enemies.clear();
    energy =0;
    // Reset lại enemy ban đầu, đảm bảo không gần người chơi
    int newEnemyX;
    do {
        newEnemyX = rand() % (SCREEN_WIDTH - 50);
    } while (abs(newEnemyX - playerRect.x) < 200); // Đảm bảo khoảng cách an toàn

    enemies.push_back({{newEnemyX, 500, 50, 50}, 3, true});

    // Đặt lại giá trị gốc cho enemyRect (nếu cần)
    enemyRect.x = newEnemyX;
    enemyRect.y = groundY;
}

void handleEvents(bool& running) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            running = false;
        }

        if (e.type == SDL_MOUSEBUTTONDOWN && isGameOver) {
            int mouseX = e.button.x;
            int mouseY = e.button.y;

            if (mouseX > restartButton.x && mouseX < restartButton.x + restartButton.w &&
                mouseY > restartButton.y && mouseY < restartButton.y + restartButton.h) {
                // Gọi resetGame() để khởi động lại game
                resetGame();
            }

            if (mouseX > exitButton.x && mouseX < exitButton.x + exitButton.w &&
                mouseY > exitButton.y && mouseY < exitButton.y + exitButton.h) {
                // Thoát game khi bấm Exit
                SDL_Quit();
                exit(0);
            }
        }
    }
}

void updateEnemies() {
    std::vector<Enemy> newEnemies; // Danh sách enemy mới để thêm sau khi duyệt xong

    for (size_t i = 0; i < enemies.size(); i++) {
        if (enemies[i].movingRight) {
            enemies[i].rect.x += enemies[i].speed;
            if (enemies[i].rect.x + enemies[i].rect.w >= SCREEN_WIDTH) {
                enemies[i].movingRight = false;  // Đổi hướng

                // Chỉ tạo enemy mới nếu số lượng chưa vượt quá giới hạn
                if (enemies.size() + newEnemies.size() < 5) {
                    int newEnemyX;
                    do {
                        newEnemyX = rand() % (SCREEN_WIDTH - 50);
                    } while (abs(newEnemyX - playerRect.x) < 200); // Giữ khoảng cách an toàn

                    newEnemies.push_back({{newEnemyX, 500, 50, 50}, 3, true});
                }
            }
        } else {
            enemies[i].rect.x -= enemies[i].speed;
            if (enemies[i].rect.x <= 0) {
                enemies[i].movingRight = true;  // Đổi hướng

                // Chỉ tạo bản sao nếu chưa đạt số lượng tối đa
                if (enemies.size() + newEnemies.size() < 5) {
                    int newEnemyX;
                    do {
                        newEnemyX = rand() % (SCREEN_WIDTH - 50);
                    } while (abs(newEnemyX - playerRect.x) < 200); // Tránh spawn gần player

                    newEnemies.push_back({{newEnemyX, 500, 50, 50}, 3, false});
                }
            }
        }
    }

    // Thêm các enemy mới sau khi duyệt xong
    enemies.insert(enemies.end(), newEnemies.begin(), newEnemies.end());
}


void handleInput(bool& running) {
    if (isStopped) return; // Nếu đã va chạm, không nhận input nữa
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
       const Uint8* state = SDL_GetKeyboardState(NULL);

    if (state[SDL_SCANCODE_SPACE]) { // Giữ phím SPACE để nạp năng lượng
        isCharging = true;
        if (energy <maxEnergy) {
            energy += 1; // Tăng năng lượng mỗi frame
    } else {
        isCharging = false;
    }
}
   if (isStopped) return; // Nếu đã va chạm, không nhận input nữa

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

            // Khi nhấn phím SPACE, bắt đầu nạp năng lượng
            if (e.key.keysym.sym == SDLK_SPACE) {
                isCharging = true;
            }
        }

        if (e.type == SDL_KEYUP) {
            // Khi thả phím SPACE, ngừng nạp năng lượng
            if (e.key.keysym.sym == SDLK_SPACE) {
                isCharging = false;
            }
        }
    }
}
void updateEnergy() {
       static int chargeRate = 0; // Biến đếm để kiểm soát tốc độ nạp

    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    if (keystate[SDL_SCANCODE_SPACE]) { // Kiểm tra nếu SPACE đang được giữ
        isCharging = true;
    } else {
        isCharging = false; // Ngừng nạp khi thả phím
        chargeRate = 0;     // Reset bộ đếm
    }

    if (isCharging && energy < maxEnergy) {
        chargeRate++;
        if (chargeRate % 5 == 0) { // Chỉ tăng năng lượng mỗi 5 frame
            energy += 1;
        }
    }

}
void updateJump() {
    if (isJumping&& !isStopped) {
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
void renderEnergyBar() {
    if( isGameOver)return ;
    SDL_Rect energyBarOutline = {50, 50, 200, 20}; // Viền thanh năng lượng
    SDL_Rect energyBarFill = {50, 50, (energy * 200) / maxEnergy, 20}; // Thanh năng lượng theo %

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
    SDL_Rect energyBar = {50, 50, energy * 2, 20}; // Thanh năng lượng
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Màu xanh lá
    SDL_RenderFillRect(renderer, &energyBar);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Reset màu vẽ
    if (isGameOver) {
        SDL_RenderCopy(renderer, gameOverTexture, NULL, NULL); // Hiển thị Game Over
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Màu đỏ cho nút
        if (restartButtonTexture) {
            SDL_RenderCopy(renderer, restartButtonTexture, NULL, &restartButton);
        }
         if (exitButtonTexture) {
            SDL_RenderCopy(renderer, exitButtonTexture, NULL, &exitButton);
        }
    }

    else{
    if (backgroundTexture) {
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
    }

    if (playerTexture) {
        SDL_RenderCopy(renderer, playerTexture, NULL, &playerRect);
    }
    for (const Enemy& enemy : enemies) {
       SDL_RenderCopy(renderer, enemyTexture, NULL, &enemy.rect);
    }

    }
     renderEnergyBar();
    SDL_RenderPresent(renderer);
}


void cleanUp() {
    SDL_DestroyTexture(playerIdleTexture);
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(gameOverTexture);
    SDL_DestroyTexture(restartButtonTexture);
    SDL_DestroyTexture(exitButtonTexture);

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
 gameOverTexture = loadTexture("gameover.png");
 restartButtonTexture = loadTexture("restart.png"); // Thay thế bằng file của bạn
exitButtonTexture = loadTexture("exit.png");

if (!restartButtonTexture || !exitButtonTexture) {
    std::cout << "Failed to load button textures!\n";
    return -1;
}

if (!gameOverTexture) return -1;
if (!enemyTexture) return -1;
    bool running = true;
    enemies.push_back({{200, 500, 50, 50}, 3, true}); // Thêm kẻ địch khởi tạo
   while (running) {
    handleEvents(running);
    if(!isStopped){
    handleInput(running);
    updateJump();
    updateEnemies();  // Cập nhật vị trí quái vật
    updateEnergy();
 for(size_t i=0;i<enemies.size();i++){
    if (checkCollision(playerRect, enemies[i].rect)) {
    isStopped = true;
    isGameOver =true;
    break;
}
    }
    }
    render();
    SDL_Delay(16);
}


    cleanUp();
    return 0;
}
