#include <firmware/sys_core.h>

#include <firmware/thermistor.h>
#include <firmware/uart.h>
#include <firmware/lcd.h>
#include <firmware/keypad.h>
#include "states.h"

static const struct state_ops *state; 

void sys_init(void) {
    // Initialize components
    therm_init();
    uart_init();
    lcd_init();
    keypad_init();

    // Set initial state to login
    state = &state_login;
    state->init();
}

void sys_run(void) {
    // Required functions
    state->keypress();
    state->process();
    state->display();
    
    // Optional functions
    if (state->send) state->send();
    if (state->receive) state->receive();
}

void sys_change_state(const struct state_ops *new_state) {
    if (state->exit) state->exit();
    if (new_state->init) new_state->init();
    state = new_state;
}
