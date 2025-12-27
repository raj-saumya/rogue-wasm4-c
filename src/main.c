#include "core/types.h"
#include "ui/screens.h"
#include "wasm4.h"

GameContext gc;

void start() {
  PALETTE[0] = 0xd6455e;
  PALETTE[1] = 0xff4d67;
  PALETTE[2] = 0x172038;
  PALETTE[3] = 0x000000;

  Game_init(&gc);
}

void update() {
  uint8_t gamepad = *GAMEPAD1;

  switch (gc.screen_state) {
  case SCREEN_START:
    Screen_start_update(&gc, gamepad);
    Screen_start_render(&gc);
    break;

  case SCREEN_PLAYING:
    Screen_playing_update(&gc, gamepad);
    Screen_playing_render(&gc);
    break;

  case SCREEN_BUFF_SELECT:
    Screen_buff_update(&gc, gamepad);
    Screen_buff_render(&gc);
    break;

  case SCREEN_GAME_OVER:
    Screen_gameover_update(&gc, gamepad);
    Screen_gameover_render(&gc);
    break;
  }
}
