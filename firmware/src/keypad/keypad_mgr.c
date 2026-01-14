#include <firmware/keypad.h>

#include "keypad_hal.h"

void keypad_mgr_init(void) {
    keypad_init();
}

struct keypad_state keypad_mgr_read(void) {
    static char previous_key;
    struct keypad_state current_state;

    current_state.last_key = previous_key;
    current_state.current_key = keypad_read();
    previous_key = current_state.current_key;

    return current_state;
}
