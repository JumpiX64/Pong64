#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdint.h>
#include <libdragon.h>

#define SCREEN_WIDTH        640
#define SCREEN_HEIGHT       480

#define BALL_START_X        320
#define BALL_START_Y        240
#define BALL_SPEED_INITIAL  4.0f
#define BALL_GRAVITY        0.35f
#define BALL_BOUNCE_FACTOR  1.01f

#define PADDLE_Y_START      200
#define PADDLE_HEIGHT       60
#define PADDLE_STEP         5
#define PADDLE_Y_MIN        40
#define PADDLE_Y_MAX        420

#define WIN_SCORE           5
#define WALL_HIT_BOOST      0.5f
#define WALL_HIT_TRIGGER    5

#define LOGO_MAX_FRAMES     60
#define LOGO_FADE_DURATION  10

#define FPS_WARN_DURATION   50

#define ENDLESS_MODE_HIGH_SCORE 0  // placeholder for now

#define CHANNEL_MUSIC   0
#define CHANNEL_SFX1    1
#define CHANNEL_SFX2    2
#define CHANNEL_SFX4    4

#define BALL_GROUND_Y   325
#define BALL_TOP_Y      1
#define BALL_TOP_PAUSE  6

typedef enum {
    STATE_LOGO,
    STATE_TITLE,
    STATE_GAME,
    STATE_RULES,
    STATE_GAME2,
    STATE_GAMEAI,
    STATE_GAMEAIE,
    STATE_GAMEAIR,
    STATE_ENDLESS,
    STATE_VICTORY
} GameState;

void rumble_tick(int *frames, joypad_port_t port) {
    if (*frames > 0 && --(*frames) == 0)
        joypad_set_rumble_active(port, false);
}

int main(void)
{
    int score_p1 = 0;
    int score_p2 = 0;
    int wall_hits = 0;
    GameState state = STATE_LOGO;

    display_init(RESOLUTION_640x480, DEPTH_16_BPP, 2, GAMMA_NONE, FILTERS_DISABLED);
    dfs_init(DFS_DEFAULT_LOCATION);
    joypad_init();
    timer_init();

    audio_init(22050, 2);  
    mixer_init(8);
    wav64_init_compression(3);

    sprite_t *custom_font = sprite_load("rom:/libdragon-font.sprite");
    graphics_set_font_sprite(custom_font);

    sprite_t *pong1 = sprite_load("rom:/pong1.sprite");
    sprite_t *pong2 = sprite_load("rom:/pong2.sprite");
    sprite_t *ball = sprite_load("rom:/ball.sprite");
    sprite_t *pongf = sprite_load("rom:/pongf.sprite");
    sprite_t *pokal = sprite_load("rom:/pokal.sprite");
    sprite_t *lib = sprite_load("rom:/lib.sprite");

    wav64_t title_music;
    wav64_open(&title_music, "rom:/menu.wav64");
    wav64_set_loop(&title_music, true);
    
    wav64_t sfx_hit;
    wav64_t sfx_victory;
    wav64_t sfx_score;
    wav64_open(&sfx_hit, "rom:/hit.wav64");
    wav64_open(&sfx_victory, "rom:/victory.wav64");
    wav64_open(&sfx_score, "rom:/score.wav64");
    
    bool music_playing = false;
    bool victory_sound_played = false;
    int rumble_p1_frames = 0;
    int rumble_p2_frames = 0;  
    
    bool show_fps = false;
    uint32_t fps_counter = 0;
    uint32_t fps_display = 0;
    uint32_t last_fps_update = 0;
    char fps_str[16] = "FPS: 0";
    int fps_warn_frames = 0; 

    float ball_x = 75, ball_y = 100, ball_speed = 5.0f;
    int top_pause_frames = 0;

    int logo_frame_cnt = 0;

    float pong1_y = PADDLE_Y_START, pong2_y = PADDLE_Y_START;
    float ball_game_x = BALL_START_X, ball_game_y = BALL_START_Y;
    float ball_dx = BALL_SPEED_INITIAL, ball_dy = 0.0f;

    while (1)
    {
        surface_t* disp = display_get();
        graphics_fill_screen(disp, 0x0);
        graphics_set_color(0xFFFFFFFF, 0x0);

        controller_scan();
        joypad_poll();
        struct controller_data keys_pressed = get_keys_pressed();
        struct controller_data held = get_keys_held();

        if (keys_pressed.c[0].down) {
            show_fps = !show_fps;
            if (show_fps) fps_warn_frames = FPS_WARN_DURATION;
        }

        bool should_play_music = (state == STATE_TITLE || 
                                   state == STATE_RULES || 
                                   state == STATE_ENDLESS || 
                                   state == STATE_GAME2);
        
        if (should_play_music && !music_playing) {
            wav64_play(&title_music, CHANNEL_MUSIC);
            mixer_ch_set_vol(CHANNEL_MUSIC, 0.5f, 0.5f);
            music_playing = true;
        } else if (!should_play_music && music_playing) {
            mixer_ch_stop(CHANNEL_MUSIC);
            music_playing = false;
        }

        switch(state)
        {
            case STATE_LOGO:
                graphics_draw_sprite_trans(disp, 10, 25, lib);
                logo_frame_cnt++;

                if (logo_frame_cnt > LOGO_MAX_FRAMES) {
                    int fade_frame = logo_frame_cnt - LOGO_MAX_FRAMES;
                    int alpha = (fade_frame * 255) / LOGO_FADE_DURATION;
                    if (alpha > 255) alpha = 255;
                    graphics_fill_screen(disp, 0x0);
                    if (fade_frame >= LOGO_FADE_DURATION) state = STATE_TITLE;
                }
                break;

            case STATE_TITLE:
                graphics_draw_text(disp, 250, 50, "PONG 64");
                graphics_draw_text(disp, 175, 150, "Press Start");
                graphics_draw_text(disp, 175, 200, "Press Z for Multiplayer COM");
                graphics_draw_text(disp, 175, 250, "Press R for Rules");
                graphics_draw_text(disp, 175, 300, "Press A for Endless Mode");
                graphics_draw_text(disp, 170, 400, "Developed by JumpiX");

                graphics_draw_sprite_trans(disp, 50, 300, pongf);
                graphics_draw_sprite_trans(disp, 50, 10, pongf);
                graphics_draw_sprite_trans(disp, ball_x, ball_y, ball);

                if (top_pause_frames > 0) top_pause_frames--;
                else { ball_speed += BALL_GRAVITY; ball_y += ball_speed; }

                if (ball_y >= BALL_GROUND_Y) { ball_y = BALL_GROUND_Y; ball_speed = -ball_speed * BALL_BOUNCE_FACTOR; }
                if (ball_y <= BALL_TOP_Y) { ball_y = BALL_TOP_Y; ball_speed = 0; top_pause_frames = BALL_TOP_PAUSE; }

                if (keys_pressed.c[0].start) {
                    score_p1 = score_p2 = 0;
                    pong1_y = pong2_y = PADDLE_Y_START;
                    ball_game_x = BALL_START_X; ball_game_y = BALL_START_Y; ball_dx = BALL_SPEED_INITIAL; ball_dy = 0;
                    state = STATE_GAME;
                } else if (keys_pressed.c[0].R) state = STATE_RULES;
                else if (keys_pressed.c[0].Z) state = STATE_GAME2;
                else if (keys_pressed.c[0].A) state = STATE_ENDLESS;
                break;

            case STATE_RULES:
                graphics_draw_text(disp, 250, 50, "Rules");
                graphics_draw_text(disp, 100, 150, "It's just Pong but on the N64");
                graphics_draw_text(disp, 100, 200, "Try to get 5 Points to win.");
                graphics_draw_text(disp, 100, 250, "Press L to return to Title");
                graphics_draw_text(disp, 170, 400, "Developed by JumpiX");

                if (keys_pressed.c[0].L) state = STATE_TITLE;
                break;

            case STATE_ENDLESS:
                graphics_draw_text(disp, 250, 250, "Endless Mode");
                graphics_draw_text(disp, 100, 300, "Coming soon!");
                graphics_draw_text(disp, 100, 350, "Press L to return to Title");
                graphics_draw_text(disp, 250, 400, "Highscore: ");

                if (keys_pressed.c[0].L) state = STATE_TITLE;
                break;

            case STATE_GAME2:
                graphics_draw_text(disp, 170, 50, "Multiplayer COM mode");
                graphics_draw_text(disp, 100, 150, "Press Start for COM Easy");
                graphics_draw_text(disp, 100, 200, "Press A for COM Normal");
                graphics_draw_text(disp, 100, 250, "Press B for COM Hard");
                graphics_draw_text(disp, 100, 300, "Press L to return to Title");
                graphics_draw_text(disp, 170, 400, "Developed by JumpiX");

                if (keys_pressed.c[0].start) state = STATE_GAMEAIE;
                if (keys_pressed.c[0].A) state = STATE_GAMEAI;
                if (keys_pressed.c[0].B) state = STATE_GAMEAIR;
                if (keys_pressed.c[0].L) state = STATE_TITLE;

                if (keys_pressed.c[0].start || keys_pressed.c[0].A || keys_pressed.c[0].B) {
                    score_p1 = score_p2 = 0;
                    pong1_y = pong2_y = PADDLE_Y_START;
                    ball_game_x = BALL_START_X; ball_game_y = BALL_START_Y; ball_dx = BALL_SPEED_INITIAL; ball_dy = 0;
                }
                break;

            case STATE_GAME:
            case STATE_GAMEAI:
            case STATE_GAMEAIE:
            case STATE_GAMEAIR:
            {
                char score_str[16];
                graphics_draw_text(disp, 20, 20, "Score:");
                graphics_draw_text(disp, 450, 20, "Score:");
                sprintf(score_str, "%d", score_p1);
                graphics_draw_text(disp, 120, 20, score_str);
                sprintf(score_str, "%d", score_p2);
                graphics_draw_text(disp, 550, 20, score_str);

                int8_t joyY1 = (int8_t)held.c[0].y;
                if (joyY1 > 20 && pong1_y > PADDLE_Y_MIN) pong1_y -= PADDLE_STEP;
                if (joyY1 < -20 && pong1_y < PADDLE_Y_MAX) pong1_y += PADDLE_STEP;

                if (state == STATE_GAME) {
                    int8_t joyY2 = (int8_t)held.c[1].y;
                    if (joyY2 > 20 && pong2_y > PADDLE_Y_MIN) pong2_y -= PADDLE_STEP;
                    if (joyY2 < -20 && pong2_y < PADDLE_Y_MAX) pong2_y += PADDLE_STEP;
                } else {
                    int speed = (state == STATE_GAMEAI) ? 4 : (state == STATE_GAMEAIE) ? 1 : 6;
                    if (ball_game_y > pong2_y + 10 && pong2_y < PADDLE_Y_MAX) pong2_y += speed;
                    else if (ball_game_y < pong2_y - 10 && pong2_y > PADDLE_Y_MIN) pong2_y -= speed;
                }

                ball_game_x += ball_dx;
                ball_game_y += ball_dy;

                if (ball_game_y <= PADDLE_Y_MIN || ball_game_y >= PADDLE_Y_MAX) { 
                    ball_dy = -ball_dy; 
                    wall_hits++;
                }

                if (ball_game_x <= 60 && ball_game_y + 20 >= pong1_y && ball_game_y <= pong1_y + PADDLE_HEIGHT) {
                    ball_game_x = 60;
                    ball_dx = -ball_dx;
                    ball_dy += (ball_game_y - (pong1_y + 30)) / 10.0f;
                    wav64_play(&sfx_hit, CHANNEL_SFX1);
                    if (joypad_get_rumble_supported(JOYPAD_PORT_1)) {
                        joypad_set_rumble_active(JOYPAD_PORT_1, true);
                        rumble_p1_frames = 16;
                    }
                }

                if (ball_game_x >= 500 && ball_game_y + 20 >= pong2_y && ball_game_y <= pong2_y + PADDLE_HEIGHT) {
                    ball_game_x = 500;
                    ball_dx = -ball_dx;
                    ball_dy += (ball_game_y - (pong2_y + 30)) / 10.0f;
                    wav64_play(&sfx_hit, CHANNEL_SFX2);
                    if (joypad_get_rumble_supported(JOYPAD_PORT_2)) {
                        joypad_set_rumble_active(JOYPAD_PORT_2, true);
                        rumble_p2_frames = 16; 
                    }
                }

                if (wall_hits >= WALL_HIT_TRIGGER) {
                    ball_dx += (ball_dx > 0) ? WALL_HIT_BOOST : -WALL_HIT_BOOST;
                    ball_dy += (ball_dy > 0) ? WALL_HIT_BOOST : -WALL_HIT_BOOST;
                    wall_hits = 0;
                }

                if (ball_game_x < 0) {
                    score_p2++;
                    wav64_play(&sfx_score, CHANNEL_SFX4);
                    if (score_p2 >= WIN_SCORE) state = STATE_VICTORY;
                    else { ball_game_x = BALL_START_X; ball_game_y = BALL_START_Y; ball_dx = BALL_SPEED_INITIAL; ball_dy = 0; }
                }

                if (ball_game_x > SCREEN_WIDTH) {
                    score_p1++;
                    wav64_play(&sfx_score, CHANNEL_SFX4);
                    if (score_p1 >= WIN_SCORE) state = STATE_VICTORY;
                    else { ball_game_x = BALL_START_X; ball_game_y = BALL_START_Y; ball_dx = -BALL_SPEED_INITIAL; ball_dy = 0; }
                }

                graphics_draw_sprite_trans(disp, 30, pong1_y, pong1);
                graphics_draw_sprite_trans(disp, 500, pong2_y, pong2);
                graphics_draw_sprite_trans(disp, ball_game_x, ball_game_y, ball);
            }
            break;

            case STATE_VICTORY:
                if (!victory_sound_played) {
                    wav64_play(&sfx_victory, CHANNEL_SFX1);
                    victory_sound_played = true;
                }
                
                if (score_p1 >= WIN_SCORE) graphics_draw_text(disp, 220, 200, "Player 1 Wins!");
                else if (score_p2 >= WIN_SCORE) graphics_draw_text(disp, 220, 200, "Player 2 Wins!");

                graphics_draw_sprite_trans(disp, 320, 150, pokal);
                graphics_draw_text(disp, 120, 250, "Press L to return to Title");

                if (keys_pressed.c[0].L) {
                    state = STATE_TITLE;
                    victory_sound_played = false;
                }
                break;
        }

        rumble_tick(&rumble_p1_frames, JOYPAD_PORT_1);
        rumble_tick(&rumble_p2_frames, JOYPAD_PORT_2);

        mixer_try_play();

        fps_counter++;
        uint32_t current_time = get_ticks();
        
        if (current_time - last_fps_update >= TICKS_PER_SECOND) {
            fps_display = fps_counter;
            fps_counter = 0;
            last_fps_update = current_time;
            sprintf(fps_str, "FPS: %u", (unsigned)fps_display);
        }
        
        if (show_fps) {
            graphics_draw_text(disp, 500, 400, fps_str);
        }

        if (fps_warn_frames > 0) {
            graphics_draw_text(disp, 20, 420, "FPS display cause slowdowns!");
            fps_warn_frames--;
        }

        display_show(disp);
    }

    return 0;
}
