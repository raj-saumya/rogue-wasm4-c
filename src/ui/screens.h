#pragma once

#include "../core/types.h"

void Screen_start_update(GameContext *gc, uint8_t gamepad);
void Screen_start_render(GameContext *gc);

void Screen_playing_update(GameContext *gc, uint8_t gamepad);
void Screen_playing_render(GameContext *gc);

void Screen_buff_init(GameContext *gc);
void Screen_buff_update(GameContext *gc, uint8_t gamepad);
void Screen_buff_render(GameContext *gc);

void Screen_gameover_update(GameContext *gc, uint8_t gamepad);
void Screen_gameover_render(GameContext *gc);

const char *Buff_get_name(BuffType type);
const char *Buff_get_description(BuffType type);
void Buff_apply(GameContext *gc, BuffType type);
