#include "../assets/sprites.h"
#include "../core/config.h"
#include "../entities/rogue.h"
#include "../wasm4.h"
#include "screens.h"

void Screen_start_update(GameContext *gc, uint8_t gamepad) {
  if (gamepad & BUTTON_2) {
    gc->screen_state = SCREEN_PLAYING;
  }
}

void Screen_start_render(GameContext *gc) {
  render_floor();

  *DRAW_COLORS = 3;
  text("The Rogue", 40, 20);
  text("Of", 68, 32);
  text("Abyss", 58, 42);
  text("Press \x81 to start", 16, 130);

  Rogue_render(gc);
}
