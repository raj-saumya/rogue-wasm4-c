#pragma once

#include <stdbool.h>
#include <stdint.h>

// Constants
#define DEMON_SPRITE_W 144
#define DEMON_SPRITE_H 24
#define DEMON_SPRITE_FRAME_SIZE 24
#define DEMON_SPRITE_FRAME_COUNT 6
#define DEMON_SPRITE_ANIM_SPEED 10
#define DEMON_COUNT 10
#define DEMON_SPAWN_INTERVAL 120 // 20 seconds at 60fps
#define DEMON_MOVE_SPEED 14      // Move every Nth frame (higher = slower)
#define DEMON_SEPARATION_DIST 2  // Min distance between demons

// Types
typedef struct Player {
  int pos_x;
  int pos_y;
  int health;
  uint8_t dir;
} Player;

typedef struct Demon {
  int pos_x, pos_y;
  bool is_alive;
} Demon;

enum ScreenState {
  GAME_START_SCREEN,
  GAME_OVER_SCREEN,
  GAME_PROGRESS_SCREEN,
};

typedef struct GameContext {
  Player player;
  Demon demons[DEMON_COUNT];
  uint32_t frame_count;
  enum ScreenState screen_state;
} GameContext;

// Player Functions
void Player_init(GameContext *gc);
void Player_render(GameContext *gc);
void Player_move(GameContext *gc, int dx, int dy);

// Game Functions
void Game_init(GameContext *gc);
void Game_startScreen(GameContext *gc, uint8_t gamepad);
void Game_progressScreen(GameContext *gc, uint8_t gamepad);
void Game_overScreen(GameContext *gc, uint8_t gamepad);
void Game_spawnDemons(GameContext *gc);
void Game_PlayerHealthStatus(GameContext *gc);

// Demon Functions
void Demon_render(Demon *d, uint32_t frame_count);
void Demon_render_all(GameContext *gc);
void Demon_swarm_tracking(GameContext *gc);
