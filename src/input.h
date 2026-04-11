#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>
#include <stdbool.h>
#include <libdragon.h>

typedef struct {
// Player 1 – pressed keys (this frame only)
    bool p1_start;
    bool p1_A;
    bool p1_B;
    bool p1_L;
    bool p1_R;
    bool p1_Z;
    bool p1_down;

    int8_t p1_joy_y;
    int8_t p2_joy_y;
} InputState;


void input_init(void);

void input_poll(InputState *input);

void rumble_trigger(int player, int frames);

void rumble_update(void);

#endif // INPUT_H
