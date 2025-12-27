#pragma once

#include "../assets/sprites.h"
#include "config.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct GameContext GameContext;

typedef enum {
  WEAPON_TYPE_STAFF,    // Single shot, high damage
  WEAPON_TYPE_CROSSBOW, // Multi-shot spread
  WEAPON_TYPE_WAND,     // Rapid fire, low damage
  WEAPON_TYPE_COUNT
} WeaponType;

typedef enum {
  BUFF_DAMAGE_UP,
  BUFF_SPEED_UP,
  BUFF_HEALTH_REGEN,
  BUFF_EXTRA_PROJECTILE,
  BUFF_COOLDOWN_REDUCTION,
  BUFF_COUNT
} BuffType;

typedef struct {
  float x, y;
  float vx, vy;
  int16_t life_time;
  int8_t damage;
  ProjectileType type;
  bool is_active;
} Projectile;

typedef struct {
  WeaponType type;
  ProjectileType projectile_type;
  int8_t level;
  int8_t projectile_count;
  int8_t base_damage;
  float speed;
  int16_t cooldown_frames;
  int16_t current_cooldown;
  bool is_active;
} Weapon;

typedef struct {
  BuffType type;
  int8_t level;
  bool is_active;
} Buff;

typedef struct {
  int16_t pos_x, pos_y;
  int8_t health;
  int8_t max_health;
  uint8_t dir; // Facing direction (for sprite flip)
  float move_speed;
  Buff buffs[MAX_BUFFS];
} Rogue;

typedef struct {
  int16_t pos_x, pos_y;
  int8_t health;
  EnemyType type;
  bool is_alive;
  uint8_t state; // For animation states
} Enemy;

typedef enum {
  SCREEN_START,       // Title screen
  SCREEN_PLAYING,     // Main gameplay
  SCREEN_BUFF_SELECT, // Level up - choose a buff
  SCREEN_GAME_OVER,   // Game over summary
} ScreenState;

struct GameContext {
  // Entities
  Rogue player;
  Enemy enemies[MAX_ENEMIES];

  // Combat
  Weapon weapons[MAX_WEAPONS];
  Projectile projectiles[MAX_PROJECTILES];

  // Game state
  uint32_t frame_count;
  uint32_t score;
  uint8_t level;
  uint8_t kills_this_level;

  ScreenState screen_state;

  // Buff selection state
  BuffType buff_choices[3];
  uint8_t buff_idx;
};

void Rogue_init(GameContext *gc);
void Rogue_render(GameContext *gc);
void Rogue_move(GameContext *gc, int dx, int dy);
void Rogue_update_projectiles(GameContext *gc);
void Rogue_fire_weapons(GameContext *gc);

void Enemy_render(Enemy *enemy, uint32_t frame_count);
void Enemy_render_all(GameContext *gc);
void Enemy_swarm_tracking(GameContext *gc);
void Enemy_spawn(GameContext *gc);

void Game_init(GameContext *gc);

void Screen_start_update(GameContext *gc, uint8_t gamepad);
void Screen_start_render(GameContext *gc);
void Screen_playing_update(GameContext *gc, uint8_t gamepad);
void Screen_playing_render(GameContext *gc);
void Screen_buff_init(GameContext *gc);
void Screen_buff_update(GameContext *gc, uint8_t gamepad);
void Screen_buff_render(GameContext *gc);
void Screen_gameover_update(GameContext *gc, uint8_t gamepad);
void Screen_gameover_render(GameContext *gc);
