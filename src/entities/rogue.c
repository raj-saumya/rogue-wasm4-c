#include "rogue.h"
#include "../assets/sprites.h"
#include "../core/config.h"
#include "../wasm4.h"

#include <math.h>
#include <stdint.h>
#include <stdlib.h>

void Rogue_init(GameContext *gc) {
  gc->player.pos_x = SCREEN_SIZE / 2 - ROGUE_SPRITE.frame_size / 2;
  gc->player.pos_y = SCREEN_SIZE / 2 - ROGUE_SPRITE.height / 2;
  gc->player.dir = BLIT_2BPP;
  gc->player.health = ROGUE_BASE_HEALTH;
  gc->player.max_health = ROGUE_BASE_HEALTH;
  gc->player.move_speed = ROGUE_BASE_SPEED;

  for (int i = 0; i < MAX_BUFFS; i++) {
    gc->player.buffs[i].is_active = false;
  }

  for (int i = 0; i < MAX_WEAPONS; i++) {
    gc->weapons[i].is_active = false;
  }

  gc->weapons[0].is_active = true;
  gc->weapons[0].type = WEAPON_TYPE_STAFF;
  gc->weapons[0].projectile_type = PROJECTILE_TYPE_FIREBALL;
  gc->weapons[0].projectile_count = 4;
  gc->weapons[0].cooldown_frames = 240;
  gc->weapons[0].level = 1;

  for (int i = 0; i < MAX_PROJECTILES; i++) {
    gc->projectiles[i].is_active = false;
  }
}

void Rogue_render(GameContext *gc) {
  *DRAW_COLORS = 0x2340;

  uint32_t anim_frame =
      (gc->frame_count / ROGUE_SPRITE.anim_speed) % ROGUE_SPRITE.frame_count;
  uint32_t src_x = anim_frame * ROGUE_SPRITE.frame_size;

  blitSub(ROGUE_SPRITE.data, gc->player.pos_x, gc->player.pos_y,
          ROGUE_SPRITE.frame_size, ROGUE_SPRITE.height, src_x, 0,
          ROGUE_SPRITE.width, gc->player.dir);
}

void Rogue_move(GameContext *gc, int dx, int dy) {
  gc->player.pos_x += dx;
  gc->player.pos_y += dy;

  Rogue_render(gc);
}

void Rogue_render_health(GameContext *gc) {
  if (gc->player.health <= 0) {
    return;
  }

  int32_t size = (int32_t)(ROGUE_SPRITE.frame_size);
  int32_t x = gc->player.pos_x;
  int32_t y = gc->player.pos_y + ROGUE_SPRITE.height + 2;

  uint32_t health_width =
      (uint32_t)((gc->player.health * size) / gc->player.max_health);

  *DRAW_COLORS = 3;
  rect(x, y, health_width, 1);
}

static void render_projectiles(GameContext *gc) {
  for (int i = 0; i < MAX_PROJECTILES; i++) {
    Projectile *proj = &gc->projectiles[i];
    if (proj->is_active) {
      const SpriteInfo *sprite = &PROJECTILE_SPRITES[proj->type];

      *DRAW_COLORS = 0x323;

      uint32_t anim_frame =
          (gc->frame_count / sprite->anim_speed) % sprite->frame_count;
      uint32_t src_x = anim_frame * sprite->frame_size;

      blitSub(sprite->data, (int32_t)proj->x, (int32_t)proj->y,
              sprite->frame_size, sprite->height, src_x, 0, sprite->width,
              gc->player.dir);
    }
  }
}

void Rogue_update_projectiles(GameContext *gc) {
  for (int i = 0; i < MAX_PROJECTILES; i++) {
    Projectile *proj = &gc->projectiles[i];
    if (proj->is_active) {
      proj->life_time--;
      proj->x += proj->vx;
      proj->y += proj->vy;

      // Collision detection with enemies
      for (int j = 0; j < MAX_ENEMIES; j++) {
        Enemy *enemy = &gc->enemies[j];
        if (enemy->is_alive) {
          const HitboxInfo *hitbox = &ENEMY_HITBOXES[enemy->type];

          // AABB collision
          if (proj->x < (float)(enemy->pos_x + hitbox->width) &&
              proj->x + PROJECTILE_SPRITES[proj->type].frame_size >
                  (float)enemy->pos_x &&
              proj->y < (float)(enemy->pos_y + hitbox->height) &&
              proj->y + PROJECTILE_SPRITES[proj->type].height >
                  (float)enemy->pos_y) {

            // Hit!
            enemy->health--;
            if (enemy->health <= 0) {
              enemy->is_alive = false;
              gc->score++;
              gc->kills_this_level++;
            }

            proj->is_active = false;
            break;
          }
        }
      }

      // Deactivate if out of bounds or lifetime ends
      if (proj->is_active && (proj->life_time <= 0 || proj->x < -10 ||
                              proj->x > SCREEN_SIZE + 10 || proj->y < -10 ||
                              proj->y > SCREEN_SIZE + 10)) {
        proj->is_active = false;
      }
    }
  }

  render_projectiles(gc);
}

typedef struct {
  int index;
  float distSq;
} EnemyDist;

static int get_sorted_enemy_indices(GameContext *gc, int *indices) {
  int count = 0;
  EnemyDist dists[MAX_ENEMIES];

  for (int i = 0; i < MAX_ENEMIES; i++) {
    if (gc->enemies[i].is_alive) {
      float dx = (float)(gc->enemies[i].pos_x - gc->player.pos_x);
      float dy = (float)(gc->enemies[i].pos_y - gc->player.pos_y);
      dists[count].index = i;
      dists[count].distSq = dx * dx + dy * dy;
      count++;
    }
  }

  // TODO: Use a better sorting algorithm
  for (int i = 0; i < count - 1; i++) {
    for (int j = 0; j < count - i - 1; j++) {
      if (dists[j].distSq > dists[j + 1].distSq) {
        EnemyDist temp = dists[j];
        dists[j] = dists[j + 1];
        dists[j + 1] = temp;
      }
    }
  }

  for (int i = 0; i < count; i++) {
    indices[i] = dists[i].index;
  }
  return count;
}

void Rogue_fire_weapons(GameContext *gc) {
  int enemy_indices[MAX_ENEMIES];
  int active_enemy_count = get_sorted_enemy_indices(gc, enemy_indices);

  for (int i = 0; i < MAX_WEAPONS; i++) {
    Weapon *weapon = &gc->weapons[i];

    if (!weapon->is_active) {
      continue;
    }

    uint32_t cooldown_frames = (uint32_t)weapon->cooldown_frames;
    if (gc->frame_count % cooldown_frames != 0) {
      continue;
    }

    int projectiles_needed = weapon->projectile_count;
    int fired_count = 0;

    for (int k = 0; k < MAX_PROJECTILES; k++) {
      if (projectiles_needed <= 0) {
        break;
      }

      Projectile *proj = &gc->projectiles[k];
      if (!proj->is_active) {
        proj->is_active = true;
        proj->type = weapon->projectile_type;
        proj->life_time = PROJECTILE_LIFETIME;
        proj->x = (float)gc->player.pos_x;
        proj->y = (float)gc->player.pos_y;

        float speed = PROJECTILE_BASE_SPEED;
        float vx = 0, vy = -speed;

        if (active_enemy_count > 0) {
          int target_idx = enemy_indices[fired_count % active_enemy_count];
          Enemy *target = &gc->enemies[target_idx];

          float dx = (float)(target->pos_x - gc->player.pos_x);
          float dy = (float)(target->pos_y - gc->player.pos_y);
          float angle = atan2f(dy, dx);

          float step = (float)(M_PI / 4.0);
          float snapped = roundf(angle / step) * step;

          vx = cosf(snapped) * speed;
          vy = sinf(snapped) * speed;
        }

        proj->vx = vx;
        proj->vy = vy;

        projectiles_needed--;
        fired_count++;
      }
    }
  }
}

void Rogue_render_kills_this_level(GameContext *gc) {
  int bar_width = SCREEN_SIZE;
  int bar_x = 0;
  int bar_y = SCREEN_SIZE - 2;
  int bar_height = 2;

  *DRAW_COLORS = 0x22;
  rect(bar_x, bar_y, (uint32_t)bar_width, (uint32_t)bar_height);

  int fill = (gc->kills_this_level * bar_width) / KILLS_PER_LEVEL;
  if (fill > 0) {
    *DRAW_COLORS = 0x33;
    rect(bar_x, bar_y, (uint32_t)fill, (uint32_t)bar_height);
  }
}