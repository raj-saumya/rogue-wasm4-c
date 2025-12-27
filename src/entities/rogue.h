#pragma once

#include "../core/types.h"

void Rogue_init(GameContext *gc);
void Rogue_render(GameContext *gc);
void Rogue_move(GameContext *gc, int dx, int dy);
void Rogue_update_projectiles(GameContext *gc);
void Rogue_fire_weapons(GameContext *gc);
void Rogue_render_health(GameContext *gc);
void Rogue_render_kills_this_level(GameContext *gc);