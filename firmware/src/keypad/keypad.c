#include <firmware/keypad.h>

#include "keypad_hal.h"

void keypad_init(void) {
    keypad_hal_init();
}

struct keypad_state keypad_get_state(void) {
    static char previous_key;
    struct keypad_state current_state;
    
    // State to send to caller
    current_state.last_key = previous_key;
    current_state.current_key = keypad_hal_read();

    previous_key = current_state.current_key; // Update previous key

    return current_state;
}
