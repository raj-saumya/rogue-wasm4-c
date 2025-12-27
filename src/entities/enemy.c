#include "enemy.h"
#include "../assets/audio.h"
#include "../assets/sprites.h"
#include "../core/config.h"
#include "../wasm4.h"

#include <stdlib.h>

void Enemy_render(Enemy *enemy, uint32_t frame_count) {
    const SpriteInfo *sprite = &ENEMY_SPRITES[enemy->type];
    
    *DRAW_COLORS = 0x4320;

    uint32_t anim_frame = (frame_count / sprite->anim_speed) % sprite->frame_count;
    uint32_t src_x = anim_frame * sprite->frame_size;

    blitSub(sprite->data, enemy->pos_x, enemy->pos_y,
            sprite->frame_size, sprite->height, src_x, 0, 
            sprite->width, sprite->flags);
}

void Enemy_render_all(GameContext *gc) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (gc->enemies[i].is_alive) {
            Enemy_render(&gc->enemies[i], gc->frame_count);
        }
    }
}

bool Enemy_check_player_collision(Enemy *enemy, Rogue *player) {
    const HitboxInfo *enemy_hitbox = &ENEMY_HITBOXES[enemy->type];
    
    int dx = enemy->pos_x;
    int dy = enemy->pos_y;
    int px = player->pos_x;
    int py = player->pos_y;

    // AABB collision between enemy and player hitboxes
    if (dx < px + ROGUE_SPRITE.frame_size &&
        dx + enemy_hitbox->width > px &&
        dy < py + ROGUE_SPRITE.height &&
        dy + enemy_hitbox->height > py) {
        return true;
    }

    return false;
}

void Enemy_swarm_tracking(GameContext *gc) {
    // Skip this frame if it's not time to move yet
    if (gc->frame_count % ENEMY_MOVE_SPEED != 0) {
        return;
    }

    // Target the center of the player sprite
    int player_center_x = gc->player.pos_x + (ROGUE_SPRITE.frame_size / 2);
    int player_center_y = gc->player.pos_y + (ROGUE_SPRITE.height / 2);

    for (int i = 0; i < MAX_ENEMIES; i++) {
        Enemy *enemy = &gc->enemies[i];

        if (!enemy->is_alive) {
            continue;
        }

        if (Enemy_check_player_collision(enemy, &gc->player)) {
            gc->player.health -= 1;
            audio_player_damage();
            continue;
        }

        int next_x = enemy->pos_x;
        int next_y = enemy->pos_y;

        // Move one pixel closer to the player center
        if (next_x < player_center_x)
            next_x += 1;
        else if (next_x > player_center_x)
            next_x -= 1;

        if (next_y < player_center_y)
            next_y += 1;
        else if (next_y > player_center_y)
            next_y -= 1;

        // Push away from neighbors to prevent stacking
        int push_x = 0;
        int push_y = 0;

        if (i > 0) {
            Enemy *prev = &gc->enemies[i - 1];
            if (prev->is_alive) {
                int dx = next_x - prev->pos_x;
                int dy = next_y - prev->pos_y;
                if (abs(dx) < ENEMY_SEPARATION_DIST &&
                    abs(dy) < ENEMY_SEPARATION_DIST) {
                    push_x += (dx > 0) ? 1 : -1;
                    push_y += (dy > 0) ? 1 : -1;
                }
            }
        }

        if (i < MAX_ENEMIES - 1) {
            Enemy *next = &gc->enemies[i + 1];
            if (next->is_alive) {
                int dx = next_x - next->pos_x;
                int dy = next_y - next->pos_y;
                if (abs(dx) < ENEMY_SEPARATION_DIST &&
                    abs(dy) < ENEMY_SEPARATION_DIST) {
                    push_x += (dx > 0) ? 1 : -1;
                    push_y += (dy > 0) ? 1 : -1;
                }
            }
        }

        enemy->pos_x = (int16_t)(next_x + push_x);
        enemy->pos_y = (int16_t)(next_y + push_y);
    }
}

void Enemy_spawn(GameContext *gc) {
    if (gc->frame_count % ENEMY_SPAWN_INTERVAL != 0) {
        return;
    }

    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!gc->enemies[i].is_alive) {
            gc->enemies[i].is_alive = true;
            gc->enemies[i].health = ENEMY_BASE_HEALTH;
            gc->enemies[i].type = (EnemyType)(rand() % ENEMY_TYPE_COUNT);
            
            // Spawn at edges
            if (rand() % 2 == 0) {
                gc->enemies[i].pos_x = rand() % SCREEN_SIZE;
                gc->enemies[i].pos_y = (rand() % 2 == 0) ? 0 : SCREEN_SIZE;
            } else {
                gc->enemies[i].pos_x = (rand() % 2 == 0) ? 0 : SCREEN_SIZE;
                gc->enemies[i].pos_y = rand() % SCREEN_SIZE;
            }
            break;
        }
    }
}

