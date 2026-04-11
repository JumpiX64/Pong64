#ifndef AUDIO_H
#define AUDIO_H

#include <libdragon.h>
#include "constants.h"

void audio_system_init(void);

void audio_load_assets(void);

void audio_update_music(GameState state);

void audio_play_hit(int player);

void audio_play_score(void);

void audio_play_victory(void);

void audio_tick(void);

#endif // AUDIO_H
