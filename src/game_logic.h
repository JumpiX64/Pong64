#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "constants.h"
#include "input.h"

typedef struct {
    float pong1_y;
    float pong2_y;

    float ball_game_x;
    float ball_game_y;
    float ball_dx;
    float ball_dy;

    int score_p1;
    int score_p2;

    int wall_hits;

    float title_ball_x;
    float title_ball_y;
    float title_ball_speed;
    int   title_top_pause;
} GameData;



//Sets all game values to initial state.
void game_init(GameData *g);


 //Resets paddles and ball.direction: 1 = ball flies to the right, -1 = to the left.
void game_reset_ball(GameData *g, int direction);

// Moves both paddles (player or AI), depending on the state.
void game_update_paddles(GameData *g, const InputState *input, GameState state);

//Moves the ball one unit.
void game_update_ball(GameData *g);

//Checks for collision with upper/lower wall and increased wall_hits.
void game_check_wall_collision(GameData *g);

//Checks for paddle collisions and calculates rebound angles. Return value: 0 = no hit, 1 = paddle P1, 2 = paddle P2.
int game_check_paddle_collision(GameData *g);

//Increases ball speed after WALL_HIT_TRIGGER wall hits.
void game_apply_speed_boost(GameData *g);

//Checks if the ball has left the field, awards points, and updates the *state on a win. Return value: 0 = no point, 1 = P1 has a point, 2 = P2 has a point.
int game_check_score(GameData *g, GameState *state);

//Updates the bouncing ball animation on the title screen.
void game_update_title_ball(GameData *g);

#endif // GAME_LOGIC_H
