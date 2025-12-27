#include "../entities/rogue.h"
#include "config.h"
#include "types.h"

#include <stdint.h>
#include <stdlib.h>

void Game_init(GameContext *gc) {
  gc->frame_count = 0;
  gc->score = 0;
  gc->level = 1;
  gc->kills_this_level = 0;
  gc->screen_state = SCREEN_START;
  gc->buff_idx = 0;

  // Initialize player
  Rogue_init(gc);

  // Initialize enemies
  for (int i = 0; i < MAX_ENEMIES; i++) {
    gc->enemies[i].is_alive = false;
    gc->enemies[i].pos_x = 0;
    gc->enemies[i].pos_y = 0;
    gc->enemies[i].type = ENEMY_TYPE_GHOST;
  }
}
