#include "types.h"
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

  if (gc.screen_state == GAME_START_SCREEN) {
    // Game_startScreen(&gc, gamepad);
    Game_progressScreen(&gc, gamepad);
  } else if (gc.screen_state == GAME_PROGRESS_SCREEN) {
    Game_progressScreen(&gc, gamepad);
  } else if (gc.screen_state == GAME_OVER_SCREEN) {
    Game_overScreen(&gc, gamepad);
  }
}