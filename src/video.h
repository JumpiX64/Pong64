#ifndef VIDEO_H
#define VIDEO_H

#include <stdbool.h>
#include <libdragon.h>
#include "constants.h"
#include "game_logic.h"


void video_init(void);
void video_load_assets(void);
surface_t *video_begin_frame(void);
void video_end_frame(surface_t *disp);

void video_draw_logo(surface_t *disp, int frame);
void video_draw_title(surface_t *disp, const GameData *g);
void video_draw_rules(surface_t *disp);
void video_draw_endless(surface_t *disp);
void video_draw_game2(surface_t *disp);
void video_draw_game(surface_t *disp, const GameData *g);
void video_draw_victory(surface_t *disp, int score_p1, int score_p2);
void video_draw_fps(surface_t *disp, bool show, int warn_frames, const char *fps_str);

#endif // VIDEO_H
