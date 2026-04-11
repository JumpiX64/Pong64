#include "game_logic.h"

void game_init(GameData *g)
{
    g->score_p1        = 0;
    g->score_p2        = 0;
    g->wall_hits       = 0;

    g->title_ball_x     = TITLE_BALL_X_START;
    g->title_ball_y     = TITLE_BALL_Y_START;
    g->title_ball_speed = TITLE_BALL_SPEED_START;
    g->title_top_pause  = 0;

    game_reset_ball(g, 1);
}

void game_reset_ball(GameData *g, int direction)
{
    g->pong1_y     = PADDLE_Y_START;
    g->pong2_y     = PADDLE_Y_START;
    g->ball_game_x = BALL_START_X;
    g->ball_game_y = BALL_START_Y;
    g->ball_dx     = (direction >= 0) ? BALL_SPEED_INITIAL : -BALL_SPEED_INITIAL;
    g->ball_dy     = 0.0f;
}

void game_update_paddles(GameData *g, const InputState *input, GameState state)
{
    if (input->p1_joy_y >  JOYSTICK_DEADZONE && g->pong1_y > PADDLE_Y_MIN) g->pong1_y -= PADDLE_STEP;
    if (input->p1_joy_y < -JOYSTICK_DEADZONE && g->pong1_y < PADDLE_Y_MAX) g->pong1_y += PADDLE_STEP;

    if (state == STATE_GAME) {
        if (input->p2_joy_y >  JOYSTICK_DEADZONE && g->pong2_y > PADDLE_Y_MIN) g->pong2_y -= PADDLE_STEP;
        if (input->p2_joy_y < -JOYSTICK_DEADZONE && g->pong2_y < PADDLE_Y_MAX) g->pong2_y += PADDLE_STEP;
    } else {
		
        int ai_speed = (state == STATE_GAMEAI)  ? AI_SPEED_NORMAL
                     : (state == STATE_GAMEAIE) ? AI_SPEED_EASY
                     : AI_SPEED_HARD;

        if (g->ball_game_y > g->pong2_y + AI_TRACKING_OFFSET && g->pong2_y < PADDLE_Y_MAX)
            g->pong2_y += ai_speed;
        else if (g->ball_game_y < g->pong2_y - AI_TRACKING_OFFSET && g->pong2_y > PADDLE_Y_MIN)
            g->pong2_y -= ai_speed;
    }
}


void game_update_ball(GameData *g)
{
    g->ball_game_x += g->ball_dx;
    g->ball_game_y += g->ball_dy;
}

void game_check_wall_collision(GameData *g)
{
    if (g->ball_game_y <= PADDLE_Y_MIN || g->ball_game_y >= PADDLE_Y_MAX) {
        g->ball_dy = -g->ball_dy;
        g->wall_hits++;
    }
}

int game_check_paddle_collision(GameData *g)
{
    if (g->ball_game_x <= BALL_LEFT_BOUND                       &&
        g->ball_game_y + BALL_SPRITE_HALF >= g->pong1_y         &&
        g->ball_game_y                    <= g->pong1_y + PADDLE_HEIGHT)
    {
        g->ball_game_x = BALL_LEFT_BOUND;
        g->ball_dx     = -g->ball_dx;
        g->ball_dy    += (g->ball_game_y - (g->pong1_y + PADDLE_CENTER_OFFSET)) / BALL_ANGLE_DIVISOR;
        return 1;
    }

    if (g->ball_game_x >= BALL_RIGHT_BOUND                      &&
        g->ball_game_y + BALL_SPRITE_HALF >= g->pong2_y         &&
        g->ball_game_y                    <= g->pong2_y + PADDLE_HEIGHT)
    {
        g->ball_game_x = BALL_RIGHT_BOUND;
        g->ball_dx     = -g->ball_dx;
        g->ball_dy    += (g->ball_game_y - (g->pong2_y + PADDLE_CENTER_OFFSET)) / BALL_ANGLE_DIVISOR;
        return 2;
    }

    return 0;
}

void game_apply_speed_boost(GameData *g)
{
    if (g->wall_hits >= WALL_HIT_TRIGGER) {
        g->ball_dx  += (g->ball_dx > 0) ? WALL_HIT_BOOST : -WALL_HIT_BOOST;
        g->ball_dy  += (g->ball_dy > 0) ? WALL_HIT_BOOST : -WALL_HIT_BOOST;
        g->wall_hits = 0;
    }
}

int game_check_score(GameData *g, GameState *state)
{
    if (g->ball_game_x < 0) {
        g->score_p2++;
        if (g->score_p2 >= WIN_SCORE)
            *state = STATE_VICTORY;
        else
            game_reset_ball(g, 1);
        return 2;
    }

    if (g->ball_game_x > SCREEN_WIDTH) {
        g->score_p1++;
        if (g->score_p1 >= WIN_SCORE)
            *state = STATE_VICTORY;
        else
            game_reset_ball(g, -1);
        return 1;
    }

    return 0;
}

void game_update_title_ball(GameData *g)
{
    if (g->title_top_pause > 0) {
        g->title_top_pause--;
    } else {
        g->title_ball_speed += BALL_GRAVITY;
        g->title_ball_y     += g->title_ball_speed;
    }

    if (g->title_ball_y >= BALL_GROUND_Y) {
        g->title_ball_y     = BALL_GROUND_Y;
        g->title_ball_speed = -g->title_ball_speed * BALL_BOUNCE_FACTOR;
    }

    if (g->title_ball_y <= BALL_TOP_Y) {
        g->title_ball_y     = BALL_TOP_Y;
        g->title_ball_speed = 0;
        g->title_top_pause  = BALL_TOP_PAUSE;
    }
}
