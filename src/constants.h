#ifndef CONSTANTS_H
#define CONSTANTS_H

//Screen
#define SCREEN_WIDTH            640
#define SCREEN_HEIGHT           480
#define DISPLAY_BUFFERS         2


// Titelscreen-Ball
#define TITLE_BALL_X_START      75.0f
#define TITLE_BALL_Y_START      100.0f
#define TITLE_BALL_SPEED_START  5.0f
#define BALL_GRAVITY            0.35f
#define BALL_BOUNCE_FACTOR      1.01f
#define BALL_GROUND_Y           325
#define BALL_TOP_Y              1
#define BALL_TOP_PAUSE          6


// Ball_Gameplay
#define BALL_START_X            320
#define BALL_START_Y            240
#define BALL_SPEED_INITIAL      4.0f
#define BALL_LEFT_BOUND         60
#define BALL_RIGHT_BOUND        500
#define BALL_ANGLE_DIVISOR      10.0f
#define BALL_SPRITE_HALF        20 


// Paddles
#define PADDLE_Y_START          200
#define PADDLE_HEIGHT           60
#define PADDLE_STEP             5
#define PADDLE_Y_MIN            40
#define PADDLE_Y_MAX            420
#define PADDLE_P1_X             30
#define PADDLE_P2_X             500
#define PADDLE_CENTER_OFFSET    30


#define AI_SPEED_EASY           1
#define AI_SPEED_NORMAL         4
#define AI_SPEED_HARD           6
#define AI_TRACKING_OFFSET      10


// Rules
#define WIN_SCORE               5
#define WALL_HIT_BOOST          0.5f
#define WALL_HIT_TRIGGER        5


// Logo / Fade
#define LOGO_MAX_FRAMES         60
#define LOGO_FADE_DURATION      10
#define ALPHA_MAX               255

// FPS-counter
#define FPS_WARN_DURATION       50
#define FPS_STR_LEN             16

// Audio
#define AUDIO_SAMPLE_RATE       22050
#define AUDIO_BUFFERS           2
#define MIXER_CHANNELS          8
#define AUDIO_COMPRESSION       3
#define MUSIC_VOLUME            0.5f

#define CHANNEL_MUSIC           0
#define CHANNEL_SFX1            1
#define CHANNEL_SFX2            2
#define CHANNEL_SFX4            4


#define JOYSTICK_DEADZONE       20

#define RUMBLE_DURATION_FRAMES  16

typedef enum {
    STATE_LOGO,
    STATE_TITLE,
    STATE_RULES,
    STATE_GAME2,
    STATE_GAME,
    STATE_GAMEAI,
    STATE_GAMEAIE,
    STATE_GAMEAIR,
    STATE_ENDLESS,
    STATE_VICTORY
} GameState;

#endif // CONSTANTS_H
