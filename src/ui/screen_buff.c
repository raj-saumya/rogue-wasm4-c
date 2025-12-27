#include "../core/config.h"
#include "../wasm4.h"
#include "screens.h"

#include <stdlib.h>

static const char *BUFF_NAMES[] = {"Damage Up", "Speed Up", "Health Regen",
                                   "Extra Shot", "Fast Reload"};

static const char *BUFF_DESCRIPTIONS[] = {"+20% damage", "+15% move speed",
                                          "+10 HP", "+1 projectile",
                                          "-20% cooldown"};

const char *Buff_get_name(BuffType type) {
  if (type < BUFF_COUNT) {
    return BUFF_NAMES[type];
  }
  return "Unknown";
}

const char *Buff_get_description(BuffType type) {
  if (type < BUFF_COUNT) {
    return BUFF_DESCRIPTIONS[type];
  }
  return "";
}

void Buff_apply(GameContext *gc, BuffType type) {
  switch (type) {
  case BUFF_DAMAGE_UP:
    for (int i = 0; i < MAX_WEAPONS; i++) {
      if (gc->weapons[i].is_active) {
        gc->weapons[i].base_damage += 1;
      }
    }
    break;

  case BUFF_SPEED_UP:
    gc->player.move_speed += 0.15f;
    break;

  case BUFF_HEALTH_REGEN:
    gc->player.health += 10;
    if (gc->player.health > gc->player.max_health) {
      gc->player.health = gc->player.max_health;
    }
    break;

  case BUFF_EXTRA_PROJECTILE:
    for (int i = 0; i < MAX_WEAPONS; i++) {
      if (gc->weapons[i].is_active) {
        gc->weapons[i].projectile_count += 1;
      }
    }
    break;

  case BUFF_COOLDOWN_REDUCTION:
    for (int i = 0; i < MAX_WEAPONS; i++) {
      if (gc->weapons[i].is_active) {
        gc->weapons[i].cooldown_frames =
            (int16_t)(gc->weapons[i].cooldown_frames * 0.8f);
      }
    }
    break;

  default:
    break;
  }

  for (int i = 0; i < MAX_BUFFS; i++) {
    if (!gc->player.buffs[i].is_active) {
      gc->player.buffs[i].is_active = true;
      gc->player.buffs[i].type = type;
      gc->player.buffs[i].level = 1;
      break;
    } else if (gc->player.buffs[i].type == type) {
      gc->player.buffs[i].level++;
      break;
    }
  }
}

void Screen_buff_init(GameContext *gc) {
  gc->buff_idx = 0;
  gc->level++;
  gc->kills_this_level = 0;

  for (int i = 0; i < 3; i++) {
    gc->buff_choices[i] = (BuffType)(rand() % BUFF_COUNT);
  }
}

static uint8_t prev_gamepad = 0;

void Screen_buff_update(GameContext *gc, uint8_t gamepad) {
  uint8_t pressed = gamepad & ~prev_gamepad;
  prev_gamepad = gamepad;

  if (pressed & BUTTON_UP) {
    if (gc->buff_idx > 0) {
      gc->buff_idx--;
    }
  }

  if (pressed & BUTTON_DOWN) {
    if (gc->buff_idx < 2) {
      gc->buff_idx++;
    }
  }

  if (pressed & BUTTON_2) {
    Buff_apply(gc, gc->buff_choices[gc->buff_idx]);
    gc->screen_state = SCREEN_PLAYING;
  }
}

void Screen_buff_render(GameContext *gc) {
  *DRAW_COLORS = 0x11;
  rect(0, 0, SCREEN_SIZE, SCREEN_SIZE);

  *DRAW_COLORS = 3;
  text("LEVEL UP!", 44, 8);
  text("Choose a buff:", 24, 20);

  for (int i = 0; i < 3; i++) {
    int y = 40 + (i * 30);
    bool selected = (gc->buff_idx == i);

    if (selected) {
      *DRAW_COLORS = 0x32;
      rect(8, y - 2, 144, 30);
    }

    *DRAW_COLORS = 3;
    text(Buff_get_name(gc->buff_choices[i]), 12, y + 4);

    *DRAW_COLORS = 3;
    text(Buff_get_description(gc->buff_choices[i]), 12, y + 16);
  }

  *DRAW_COLORS = 0x31;
  rect(8, 140, 144, 10);
  text("Press \x81 to Confirm", 8, 140);
}
