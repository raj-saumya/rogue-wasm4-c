#include "audio.h"
#include "../wasm4.h"

void audio_player_move(void) { tone(4, 4, 20, TONE_PULSE1); }

void audio_player_attack(void) {
  tone(60 | (110 << 16), (15 << 24) | (10 << 0), 50,
       TONE_NOISE | (TONE_MODE1 << 2));
}

void audio_player_damage(void) {
  tone(120 | (60 << 16), 15, 80, TONE_TRIANGLE);
}

void audio_enemy_hit(void) { tone(400 | (100 << 16), 6, 50, TONE_NOISE); }

void audio_enemy_death(void) { tone(300 | (50 << 16), 12, 60, TONE_NOISE); }

void audio_game_intro(void) {}

void audio_game_over(void) {
  tone(162 | (100 << 16), 20, 70, TONE_PULSE1 | TONE_MODE3);
}

void audio_level_up(void) {
  tone(262 | (523 << 16), 16, 50, TONE_PULSE1 | TONE_MODE3);
}
