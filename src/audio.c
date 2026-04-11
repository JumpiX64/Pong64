#include "audio.h"
#include "constants.h"
#include <libdragon.h>
#include <stdbool.h>

static wav64_t title_music;
static wav64_t sfx_hit;
static wav64_t sfx_victory;
static wav64_t sfx_score;

static bool music_playing = false;

void audio_system_init(void)
{
    audio_init(AUDIO_SAMPLE_RATE, AUDIO_BUFFERS);
    mixer_init(MIXER_CHANNELS);
    wav64_init_compression(AUDIO_COMPRESSION);
}

void audio_load_assets(void)
{
    wav64_open(&title_music, "rom:/menu.wav64");
    wav64_set_loop(&title_music, true);

    wav64_open(&sfx_hit,     "rom:/hit.wav64");
    wav64_open(&sfx_victory, "rom:/victory.wav64");
    wav64_open(&sfx_score,   "rom:/score.wav64");
}

void audio_update_music(GameState state)
{
    bool should_play = (state == STATE_TITLE   ||
                        state == STATE_RULES   ||
                        state == STATE_ENDLESS ||
                        state == STATE_GAME2);

    if (should_play && !music_playing) {
        wav64_play(&title_music, CHANNEL_MUSIC);
        mixer_ch_set_vol(CHANNEL_MUSIC, MUSIC_VOLUME, MUSIC_VOLUME);
        music_playing = true;
    } else if (!should_play && music_playing) {
        mixer_ch_stop(CHANNEL_MUSIC);
        music_playing = false;
    }
}

void audio_play_hit(int player)
{
    wav64_play(&sfx_hit, (player == 1) ? CHANNEL_SFX1 : CHANNEL_SFX2);
}

void audio_play_score(void)
{
    wav64_play(&sfx_score, CHANNEL_SFX4);
}

void audio_play_victory(void)
{
    wav64_play(&sfx_victory, CHANNEL_SFX1);
}

void audio_tick(void)
{
    mixer_try_play();
}
