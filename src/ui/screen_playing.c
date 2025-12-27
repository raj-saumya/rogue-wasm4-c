#include "../assets/audio.h"
#include "../assets/sprites.h"
#include "../core/config.h"
#include "../entities/enemy.h"
#include "../entities/rogue.h"
#include "../wasm4.h"
#include "screens.h"

void Screen_playing_update(GameContext *gc, uint8_t gamepad) {
  gc->frame_count++;

  render_floor();

  // Player movement
  if (gamepad & BUTTON_LEFT) {
    gc->player.dir = BLIT_2BPP | BLIT_FLIP_X;
    Rogue_move(gc, -1, 0);
    audio_player_move();
  } else if (gamepad & BUTTON_RIGHT) {
    gc->player.dir = BLIT_2BPP;
    Rogue_move(gc, 1, 0);
    audio_player_move();
  } else if (gamepad & BUTTON_UP) {
    Rogue_move(gc, 0, -1);
    audio_player_move();
  } else if (gamepad & BUTTON_DOWN) {
    Rogue_move(gc, 0, 1);
    audio_player_move();
  } else {
    Rogue_render(gc);
  }

  // Enemy spawning and movement
  Enemy_spawn(gc);
  Enemy_swarm_tracking(gc);
  Enemy_render_all(gc);

  // Combat
  Rogue_update_projectiles(gc);
  Rogue_fire_weapons(gc);

  // Check player death
  if (gc->player.health <= 0) {
    audio_game_over();
    gc->screen_state = SCREEN_GAME_OVER;
    return;
  }

  // Check level up
  if (gc->kills_this_level >= KILLS_PER_LEVEL) {
    audio_level_up();
    Screen_buff_init(gc);
    gc->screen_state = SCREEN_BUFF_SELECT;
    return;
  }
}

void Screen_playing_render(GameContext *gc) {
  Rogue_render_health(gc);
  Rogue_render_kills_this_level(gc);
}
