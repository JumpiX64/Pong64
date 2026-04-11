#include "input.h"
#include "constants.h"

static int rumble_p1_frames = 0;
static int rumble_p2_frames = 0;

void input_init(void)
{
    joypad_init();
}

void input_poll(InputState *input)
{
    controller_scan();
    joypad_poll();

    struct controller_data pressed = get_keys_pressed();
    struct controller_data held    = get_keys_held();

    input->p1_start = (bool)pressed.c[0].start;
    input->p1_A     = (bool)pressed.c[0].A;
    input->p1_B     = (bool)pressed.c[0].B;
    input->p1_L     = (bool)pressed.c[0].L;
    input->p1_R     = (bool)pressed.c[0].R;
    input->p1_Z     = (bool)pressed.c[0].Z;
    input->p1_down  = (bool)pressed.c[0].down;
    
    input->p1_joy_y = (int8_t)held.c[0].y;
    input->p2_joy_y = (int8_t)held.c[1].y;
}

void rumble_trigger(int player, int frames)
{
    if (player == 1 && joypad_get_rumble_supported(JOYPAD_PORT_1)) {
        joypad_set_rumble_active(JOYPAD_PORT_1, true);
        rumble_p1_frames = frames;
    } else if (player == 2 && joypad_get_rumble_supported(JOYPAD_PORT_2)) {
        joypad_set_rumble_active(JOYPAD_PORT_2, true);
        rumble_p2_frames = frames;
    }
}

void rumble_update(void)
{
    if (rumble_p1_frames > 0 && --rumble_p1_frames == 0)
        joypad_set_rumble_active(JOYPAD_PORT_1, false);

    if (rumble_p2_frames > 0 && --rumble_p2_frames == 0)
        joypad_set_rumble_active(JOYPAD_PORT_2, false);
}
