#include "screens.h"
#include "../core/config.h"
#include "../wasm4.h"

void Game_init(GameContext *gc);

void Screen_gameover_update(GameContext *gc, uint8_t gamepad) {
  if (gamepad & BUTTON_2) {
    Game_init(gc);
    gc->screen_state = SCREEN_PLAYING;
  }
}

void Screen_gameover_render(GameContext *gc) {
  // Simple dark background
  *DRAW_COLORS = 0x11;
  rect(0, 0, SCREEN_SIZE, SCREEN_SIZE);

  *DRAW_COLORS = 3;
  text("Game Over", 40, 10);

  // Scorecard
  char level_str[16] = "Level: ";
  char num[8];
  int idx = 0;
  int val = gc->level;
  if (val == 0) {
    num[idx++] = '0';
  } else {
    while (val > 0 && idx < 6) {
      num[idx++] = (char)((val % 10) + '0');
      val /= 10;
    }
  }
  // Reverse
  for (int j = 0; j < idx / 2; j++) {
    char tmp = num[j];
    num[j] = num[idx - 1 - j];
    num[idx - 1 - j] = tmp;
  }
  num[idx] = '\0';
  // Append
  int len = 7;
  for (int j = 0; j <= idx; j++) {
    level_str[len + j] = num[j];
  }
  text(level_str, 48, 40);

  // Score (kills)
  char score_str[32] = "Enemies Slain: ";
  idx = 0;
  val = (int)gc->score;
  if (val == 0) {
    num[idx++] = '0';
  } else {
    while (val > 0 && idx < 6) {
      num[idx++] = (char)((val % 10) + '0');
      val /= 10;
    }
  }
  for (int j = 0; j < idx / 2; j++) {
    char tmp = num[j];
    num[j] = num[idx - 1 - j];
    num[idx - 1 - j] = tmp;
  }
  num[idx] = '\0';
  len = 15;
  for (int j = 0; j <= idx; j++) {
    score_str[len + j] = num[j];
  }
  text(score_str, 8, 55);

  // Count active buffs
  int buff_count = 0;
  for (int i = 0; i < MAX_BUFFS; i++) {
    if (gc->player.buffs[i].is_active) {
      buff_count++;
    }
  }
  char buff_str[16] = "Buffs: ";
  num[0] = (char)(buff_count + '0');
  num[1] = '\0';
  buff_str[7] = num[0];
  buff_str[8] = '\0';
  text(buff_str, 48, 70);

  text("Press \x81 to restart", 8, 130);
}
