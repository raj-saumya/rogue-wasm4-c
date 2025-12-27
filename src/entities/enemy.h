#pragma once

#include "../core/types.h"

void Enemy_render(Enemy *enemy, uint32_t frame_count);
void Enemy_render_all(GameContext *gc);
void Enemy_swarm_tracking(GameContext *gc);
void Enemy_spawn(GameContext *gc);
bool Enemy_check_player_collision(Enemy *enemy, Rogue *player);
