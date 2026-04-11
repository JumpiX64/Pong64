#include "video.h"
#include <stdio.h>

static sprite_t *spr_font;
static sprite_t *spr_pong1;
static sprite_t *spr_pong2;
static sprite_t *spr_ball;
static sprite_t *spr_pongf;
static sprite_t *spr_pokal;
static sprite_t *spr_lib;

void video_init(void)
{
    display_init(RESOLUTION_640x480, DEPTH_16_BPP, DISPLAY_BUFFERS, GAMMA_NONE, FILTERS_DISABLED);
}

void video_load_assets(void)
{
    spr_font  = sprite_load("rom:/libdragon-font.sprite");
    graphics_set_font_sprite(spr_font);

    spr_pong1 = sprite_load("rom:/pong1.sprite");
    spr_pong2 = sprite_load("rom:/pong2.sprite");
    spr_ball  = sprite_load("rom:/ball.sprite");
    spr_pongf = sprite_load("rom:/pongf.sprite");
    spr_pokal = sprite_load("rom:/pokal.sprite");
    spr_lib   = sprite_load("rom:/lib.sprite");
}

surface_t *video_begin_frame(void)
{
    surface_t *disp = display_get();
    graphics_fill_screen(disp, 0x0);
    graphics_set_color(0xFFFFFFFF, 0x0);
    return disp;
}

void video_end_frame(surface_t *disp)
{
    display_show(disp);
}

void video_draw_logo(surface_t *disp, int frame)
{
    graphics_draw_sprite_trans(disp, 10, 25, spr_lib);

    if (frame > LOGO_MAX_FRAMES) {
        int fade  = frame - LOGO_MAX_FRAMES;
        int alpha = (fade * ALPHA_MAX) / LOGO_FADE_DURATION;
        if (alpha > ALPHA_MAX) alpha = ALPHA_MAX;
        graphics_fill_screen(disp, 0x0);
    }
}

void video_draw_title(surface_t *disp, const GameData *g)
{
    graphics_draw_text(disp, 250, 50,  "PONG 64");
    graphics_draw_text(disp, 175, 150, "Press Start");
    graphics_draw_text(disp, 175, 200, "Press Z for Multiplayer COM");
    graphics_draw_text(disp, 175, 250, "Press R for Rules");
    graphics_draw_text(disp, 175, 300, "Press A for Endless Mode");
    graphics_draw_text(disp, 170, 400, "Developed by JumpiX");

    graphics_draw_sprite_trans(disp, 50, 300, spr_pongf);
    graphics_draw_sprite_trans(disp, 50, 10,  spr_pongf);
    graphics_draw_sprite_trans(disp, (int)g->title_ball_x, (int)g->title_ball_y, spr_ball);
}

void video_draw_rules(surface_t *disp)
{
    graphics_draw_text(disp, 250, 50,  "Rules");
    graphics_draw_text(disp, 100, 150, "It's just Pong but on the N64");
    graphics_draw_text(disp, 100, 200, "Try to get 5 Points to win.");
    graphics_draw_text(disp, 100, 250, "Press L to return to Title");
    graphics_draw_text(disp, 170, 400, "Developed by JumpiX");
}

void video_draw_endless(surface_t *disp)
{
    graphics_draw_text(disp, 250, 250, "Endless Mode");
    graphics_draw_text(disp, 100, 300, "Coming soon!");
    graphics_draw_text(disp, 100, 350, "Press L to return to Title");
    graphics_draw_text(disp, 250, 400, "Highscore: ");
}

void video_draw_game2(surface_t *disp)
{
    graphics_draw_text(disp, 170, 50,  "Multiplayer COM mode");
    graphics_draw_text(disp, 100, 150, "Press Start for COM Easy");
    graphics_draw_text(disp, 100, 200, "Press A for COM Normal");
    graphics_draw_text(disp, 100, 250, "Press B for COM Hard");
    graphics_draw_text(disp, 100, 300, "Press L to return to Title");
    graphics_draw_text(disp, 170, 400, "Developed by JumpiX");
}

void video_draw_game(surface_t *disp, const GameData *g)
{
    char score_str[16];
    graphics_draw_text(disp, 20,  20, "Score:");
    graphics_draw_text(disp, 450, 20, "Score:");

    sprintf(score_str, "%d", g->score_p1);
    graphics_draw_text(disp, 120, 20, score_str);

    sprintf(score_str, "%d", g->score_p2);
    graphics_draw_text(disp, 550, 20, score_str);

    graphics_draw_sprite_trans(disp, PADDLE_P1_X,       (int)g->pong1_y,    spr_pong1);
    graphics_draw_sprite_trans(disp, PADDLE_P2_X,       (int)g->pong2_y,    spr_pong2);
    graphics_draw_sprite_trans(disp, (int)g->ball_game_x, (int)g->ball_game_y, spr_ball);
}

void video_draw_victory(surface_t *disp, int score_p1, int score_p2)
{
    if      (score_p1 >= WIN_SCORE) graphics_draw_text(disp, 220, 200, "Player 1 Wins!");
    else if (score_p2 >= WIN_SCORE) graphics_draw_text(disp, 220, 200, "Player 2 Wins!");

    graphics_draw_sprite_trans(disp, 320, 150, spr_pokal);
    graphics_draw_text(disp, 120, 250, "Press L to return to Title");
}

void video_draw_fps(surface_t *disp, bool show, int warn_frames, const char *fps_str)
{
    if (show)
        graphics_draw_text(disp, 500, 400, fps_str);

    if (warn_frames > 0)
        graphics_draw_text(disp, 20, 420, "FPS display cause slowdowns!");
}
