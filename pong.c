#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdint.h>
#include <libdragon.h>

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

int main(void)
{
    int score_p1 = 0;
    int score_p2 = 0;
    int wall_hits = 0;
    GameState state = STATE_LOGO;

    display_init(RESOLUTION_640x480, DEPTH_16_BPP, 2, GAMMA_NONE, FILTERS_RESAMPLE);
    dfs_init(DFS_DEFAULT_LOCATION);
    controller_init();
    timer_init();

    sprite_t *custom_font = sprite_load("rom:/libdragon-font.sprite");
    graphics_set_font_sprite(custom_font);

    sprite_t *pong1 = sprite_load("rom:/pong1.sprite");
    sprite_t *pong2 = sprite_load("rom:/pong2.sprite");
    sprite_t *ball = sprite_load("rom:/ball.sprite");
    sprite_t *pongf = sprite_load("rom:/pongf.sprite");
    sprite_t *pokal = sprite_load("rom:/pokal.sprite");
    sprite_t *logol = sprite_load("rom:/logol.sprite");
    sprite_t *endless = sprite_load("rom:/endless.sprite");

    float ball_x = 75, ball_y = 100, ball_speed = 5.0f, gravity = 0.35f, bounce_factor = 1.01f;
    int ground_y = 325, top_y = 1, top_pause_frames = 0, max_top_pause = 6;
    int logo_frame_cnt = 0, logo_max_frames = 60, fade_duration = 10;

    float pong1_y = 200, pong2_y = 200;
    float ball_game_x = 320, ball_game_y = 240;
    float ball_dx = 4.0f, ball_dy = 0.0f;

    while (1)
    {
        surface_t* disp = display_get();
        graphics_fill_screen(disp, 0x0);
        graphics_set_color(0xFFFFFFFF, 0x0);

        controller_scan();
        struct controller_data keys_pressed = get_keys_pressed();
        struct controller_data held = get_keys_held();

        switch(state)
        {
            case STATE_LOGO:
                graphics_draw_sprite_trans(disp, 10, 25, logol);
                logo_frame_cnt++;

                if (logo_frame_cnt > logo_max_frames) {
                    int fade_frame = logo_frame_cnt - logo_max_frames;
                    int alpha = (fade_frame * 255) / fade_duration;
                    if (alpha > 255) alpha = 255;
                    graphics_fill_screen(disp, 0x0);
                    if (fade_frame >= fade_duration) state = STATE_TITLE;
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
                else { ball_speed += gravity; ball_y += ball_speed; }

                if (ball_y >= ground_y) { ball_y = ground_y; ball_speed = -ball_speed * bounce_factor; }
                if (ball_y <= top_y) { ball_y = top_y; ball_speed = 0; top_pause_frames = max_top_pause; }

                if (keys_pressed.c[0].start) {
                    score_p1 = score_p2 = 0;
                    pong1_y = pong2_y = 200;
                    ball_game_x = 320; ball_game_y = 240; ball_dx = 4; ball_dy = 0;
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
                graphics_draw_sprite_trans(disp, 100, 0, endless);

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
                    pong1_y = pong2_y = 200;
                    ball_game_x = 320; ball_game_y = 240; ball_dx = 4; ball_dy = 0;
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
                if (joyY1 > 20 && pong1_y > 40) pong1_y -= 5;
                if (joyY1 < -20 && pong1_y < 420) pong1_y += 5;

                if (state == STATE_GAME) {
                    int8_t joyY2 = (int8_t)held.c[1].y;
                    if (joyY2 > 20 && pong2_y > 40) pong2_y -= 5;
                    if (joyY2 < -20 && pong2_y < 420) pong2_y += 5;
                } else {
                    int speed = (state == STATE_GAMEAI) ? 4 : (state == STATE_GAMEAIE) ? 1 : 6;
                    if (ball_game_y > pong2_y + 10 && pong2_y < 420) pong2_y += speed;
                    else if (ball_game_y < pong2_y - 10 && pong2_y > 40) pong2_y -= speed;
                }

                ball_game_x += ball_dx;
                ball_game_y += ball_dy;

                if (ball_game_y <= 40 || ball_game_y >= 440) { ball_dy = -ball_dy; wall_hits++; }
                if (ball_game_x <= 60 && ball_game_y + 20 >= pong1_y && ball_game_y <= pong1_y + 60) {
                    ball_game_x = 60;
                    ball_dx = -ball_dx;
                    ball_dy += (ball_game_y - (pong1_y + 30)) / 10.0f;
                }
                if (ball_game_x >= 500 && ball_game_y + 20 >= pong2_y && ball_game_y <= pong2_y + 60) {
                    ball_game_x = 500;
                    ball_dx = -ball_dx;
                    ball_dy += (ball_game_y - (pong2_y + 30)) / 10.0f;
                }

                if (wall_hits >= 5) {
                    if (ball_dx > 0) ball_dx += 0.5f; else ball_dx -= 0.5f;
                    if (ball_dy > 0) ball_dy += 0.5f; else ball_dy -= 0.5f;
                    wall_hits = 0;
                }

                if (ball_game_x < 0) {
                    score_p2++;
                    if (score_p2 >= 5) state = STATE_VICTORY;
                    else { ball_game_x = 320; ball_game_y = 240; ball_dx = 4; ball_dy = 0; }
                }
                if (ball_game_x > 640) {
                    score_p1++;
                    if (score_p1 >= 5) state = STATE_VICTORY;
                    else { ball_game_x = 320; ball_game_y = 240; ball_dx = -4; ball_dy = 0; }
                }

                graphics_draw_sprite_trans(disp, 30, pong1_y, pong1);
                graphics_draw_sprite_trans(disp, 500, pong2_y, pong2);
                graphics_draw_sprite_trans(disp, ball_game_x, ball_game_y, ball);
            }
            break;

            case STATE_VICTORY:
                if (score_p1 >= 5) graphics_draw_text(disp, 220, 200, "Player 1 Wins!");
                else if (score_p2 >= 5) graphics_draw_text(disp, 220, 200, "Player 2 Wins!");

                graphics_draw_sprite_trans(disp, 320, 150, pokal);
                graphics_draw_text(disp, 120, 250, "Press L to return to Title");

                if (keys_pressed.c[0].L) state = STATE_TITLE;
                break;
        }

        display_show(disp);
    }

    return 0;
}

