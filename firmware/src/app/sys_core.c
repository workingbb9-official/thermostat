#include <firmware/sys_core.h>

#include <firmware/thermistor.h>
#include <firmware/uart.h>
#include <firmware/lcd.h>
#include <firmware/keypad.h>
#include "states.h"

static const struct state_ops *state; 

void sys_init(void) {
    therm_mgr_init();
    uart_mgr_init();
    lcd_mgr_init();
    keypad_mgr_init();
    
    state = &state_login;
    state->init();
}

void sys_run(void) {
    state->on_keypress();
    state->process();
    state->display();
    if (state->send) state->send();
    if (state->receive) state->receive();
}

void sys_change_state(const struct state_ops *new_state) {
    if (state->exit) state->exit();
    if (new_state->init) new_state->init();
    state = new_state;
}
