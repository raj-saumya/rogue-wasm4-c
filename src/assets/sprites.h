#pragma once

#include "../wasm4.h"
#include <stdint.h>

typedef struct {
  const uint8_t *data;
  uint16_t width;
  uint16_t height;
  uint8_t frame_size;
  uint8_t frame_count;
  uint8_t anim_speed;
  uint8_t flags;
} SpriteInfo;

typedef struct {
  uint8_t width;
  uint8_t height;
  int8_t offset_x;
  int8_t offset_y;
} HitboxInfo;

extern const SpriteInfo ROGUE_SPRITE;
extern const HitboxInfo ROGUE_HITBOX;

typedef enum {
  ENEMY_TYPE_SKULL, // Fast, low HP
  ENEMY_TYPE_GHOST, // Slow, high HP
  ENEMY_TYPE_COUNT
} EnemyType;

extern const SpriteInfo ENEMY_SPRITES[];
extern const HitboxInfo ENEMY_HITBOXES[];

typedef enum { PROJECTILE_TYPE_FIREBALL, PROJECTILE_TYPE_COUNT } ProjectileType;

extern const SpriteInfo PROJECTILE_SPRITES[];

extern const uint8_t FLOOR_TILE_DATA[];
#define FLOOR_TILE_SIZE 24

void render_floor(void);

#define SPRITE_FRAME_X(sprite, frame_count)                                    \
  (((frame_count) / (sprite).anim_speed) % (sprite).frame_count) *             \
      (sprite).frame_size
