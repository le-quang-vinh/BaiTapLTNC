#ifndef CONFIG_H
#define CONFIG_H

// màn hình
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// nhân vật
const int PLAYER_SPEED = 5;
const int JUMP_FRAMES = 60;
const double gravity = 0.5;
const double groundY = 500;

//  hiệu ứng hào quang
const int AURA_FRAME_DELAY = 5;  // Delay giữa các khung hình
const int AURA_FRAME_COUNT = 4;  // Số lượng khung hình

//  enemy
const int MAX_ENEMIES = 10;
const int enemySpeed = 3;
//  chưởng (chao)
const int CHAO_COST = 20;
const int CHAO_SPEED = 4;
const int CHAO_COOLDOWN_TIME = 20;

// Năng lượng
const int maxEnergy = 100; // maxEnergy không thay đổi trong runtime -> có thể để const

#endif
