#include "logicgame.h"
#include "global.h"
#include <cstdlib>
#include <cmath>
#include "config.h"
#include "game.h"
bool checkCollision(SDL_Rect a, SDL_Rect b) {
    return (a.x + 10 < b.x + b.w - 10 &&
            a.x + a.w - 10 > b.x + 10 &&
            a.y < b.y + b.h - 5 &&
            a.y + a.h > b.y + 5);
}

void resetGame() {
    // Đặt lại vị trí nhân vật
    playerRect.x = 100;
    playerRect.y = groundY;

    // Reset trạng thái
    isGameOver = false;
    isStopped = false;
    isJumping = false;
    gameWon = false;
    jumpVelocity = -10; // Đặt lại vận tốc nhảy

    // Xóa toàn bộ danh sách kẻ địch
    enemies.clear();
    energy =0;
    chaos.clear();
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
void shootChao() {
   if (energy >= CHAO_COST && chaoCooldown <= 0) {  // Chỉ bắn khi có đủ năng lượng và cooldown hết
        Chao newChao;
        newChao.rect.x = playerRect.x + playerRect.w;  // Bắt đầu từ vị trí nhân vật
        newChao.rect.y = playerRect.y + playerRect.h / 2 - 10; // Căn giữa chưởng
        newChao.rect.w = 35;  // Kích thước chưởng
        newChao.rect.h = 22;
        newChao.speed = CHAO_SPEED;

        chaos.push_back(newChao);
        if (chaoSound) {
    Mix_PlayChannel(-1, chaoSound, 0);
}
        energy -= CHAO_COST;  // Trừ năng lượng
        chaoCooldown = CHAO_COOLDOWN_TIME;  // Reset cooldown sau khi bắn
    }
}
void updateCooldown() {
    if (chaoCooldown > 0) {
        chaoCooldown--;  // Giảm cooldown mỗi frame
    }
}

void updateChaos() {
    for (int i = chaos.size() - 1; i >= 0; i--) {
        chaos[i].rect.x += chaos[i].speed;

        // Xóa chưởng khi ra khỏi màn hình
        if (chaos[i].rect.x > SCREEN_WIDTH) {
            chaos.erase(chaos.begin() + i);
        }
         for (int j = 0; j < enemies.size(); j++) {
            if (checkCollision(chaos[i].rect, enemies[j].rect)) {
                // Xóa kẻ địch khi bị chưởng trúng
                enemies.erase(enemies.begin() + j);
                chaos.erase(chaos.begin() + i);
                i--;  // Điều chỉnh lại chỉ mục sau khi xóa
                break;
            }
        }
         if (enemies.empty()) {
        gameWon = true;
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
   if (isCharging) {
        auraTimer++;
        if (auraTimer >= AURA_FRAME_DELAY) {
            auraTimer = 0;
            auraFrame = (auraFrame + 1) % AURA_FRAME_COUNT; // Chuyển ảnh theo vòng lặp
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
