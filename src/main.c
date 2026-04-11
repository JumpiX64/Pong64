#include <stdio.h>
#include <stdint.h>
#include <libdragon.h>

#include "constants.h"
#include "video.h"
#include "input.h"
#include "audio.h"
#include "game_logic.h"

int main(void)
{
    dfs_init(DFS_DEFAULT_LOCATION);
    timer_init();
    video_init();
    input_init();
    audio_system_init();

    video_load_assets();
    audio_load_assets();
    GameState state = STATE_LOGO;
    GameData  game;
    game_init(&game);

    bool victory_sound_played = false;
    int  logo_frame_cnt       = 0;

    bool     show_fps        = false;
    uint32_t fps_counter     = 0;
    uint32_t fps_display     = 0;
    uint32_t last_fps_update = 0;
    int      fps_warn_frames = 0;
    char     fps_str[FPS_STR_LEN] = "FPS: 0";
    
    while (1)
    {
        surface_t *disp = video_begin_frame();

        InputState input;
        input_poll(&input);

        if (input.p1_down) {
            show_fps = !show_fps;
            if (show_fps) fps_warn_frames = FPS_WARN_DURATION;
        }

        audio_update_music(state);

        switch (state)
        {
            case STATE_LOGO:
                video_draw_logo(disp, logo_frame_cnt);
                logo_frame_cnt++;
                if (logo_frame_cnt >= LOGO_MAX_FRAMES + LOGO_FADE_DURATION)
                    state = STATE_TITLE;
                break;

            case STATE_TITLE:
                game_update_title_ball(&game);
                video_draw_title(disp, &game);

                if      (input.p1_start) { game_init(&game); state = STATE_GAME;    }
                else if (input.p1_R)     {                   state = STATE_RULES;   }
                else if (input.p1_Z)     {                   state = STATE_GAME2;   }
                else if (input.p1_A)     {                   state = STATE_ENDLESS; }
                break;

            case STATE_RULES:
                video_draw_rules(disp);
                if (input.p1_L) state = STATE_TITLE;
                break;

            case STATE_ENDLESS:
                video_draw_endless(disp);
                if (input.p1_L) state = STATE_TITLE;
                break;

            case STATE_GAME2:
                video_draw_game2(disp);

                if (input.p1_start || input.p1_A || input.p1_B) {
                    game_init(&game);
                    if      (input.p1_start) state = STATE_GAMEAIE;
                    else if (input.p1_A)     state = STATE_GAMEAI;
                    else                     state = STATE_GAMEAIR;
                } else if (input.p1_L) {
                    state = STATE_TITLE;
                }
                break;

            case STATE_GAME:
            case STATE_GAMEAI:
            case STATE_GAMEAIE:
            case STATE_GAMEAIR:
            {
                game_update_paddles(&game, &input, state);
                game_update_ball(&game);
                game_check_wall_collision(&game);

                int hit = game_check_paddle_collision(&game);
                if (hit == 1) { audio_play_hit(1); rumble_trigger(1, RUMBLE_DURATION_FRAMES); }
                if (hit == 2) { audio_play_hit(2); rumble_trigger(2, RUMBLE_DURATION_FRAMES); }

                game_apply_speed_boost(&game);

                int scored = game_check_score(&game, &state);
                if (scored > 0) audio_play_score();

                video_draw_game(disp, &game);
            }
            break;
            
            case STATE_VICTORY:
                if (!victory_sound_played) {
                    audio_play_victory();
                    victory_sound_played = true;
                }
                video_draw_victory(disp, game.score_p1, game.score_p2);

                if (input.p1_L) {
                    state                = STATE_TITLE;
                    victory_sound_played = false;
                }
                break;
        }

        rumble_update();
        audio_tick();

        fps_counter++;
        uint32_t now = get_ticks();
        if (now - last_fps_update >= TICKS_PER_SECOND) {
            fps_display     = fps_counter;
            fps_counter     = 0;
            last_fps_update = now;
            sprintf(fps_str, "FPS: %u", (unsigned)fps_display);
        }
        if (fps_warn_frames > 0) fps_warn_frames--;

        video_draw_fps(disp, show_fps, fps_warn_frames, fps_str);
        video_end_frame(disp);
    }

    return 0;
}
