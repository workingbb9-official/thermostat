#include <firmware/system_core.h>

#include <firmware/thermistor.h>
#include <firmware/uart.h>
#include <firmware/lcd.h>
#include <firmware/keypad.h>
#include "states.h"
#include "state_stats.h"

static enum sys_state current_state = STATE_LOGIN;
static const struct state_actions *current_actions; 

void system_init(void) {
    therm_mgr_init();
    uart_mgr_init();
    lcd_mgr_init();
    keypad_mgr_init();
    
    current_actions = &login_state;
    current_actions->init();
}

void system_run(void) {
    if (current_state == STATE_STATS) {
        stats_run(&current_state);
        return;
    }


    current_actions->on_keypress(&current_state);
    current_actions->process();
    current_actions->display();
    if (current_actions->send) current_actions->send();
    if (current_actions->receive) current_actions->receive();
}

void sys_change_state(const struct state_actions *new_state) {
    if (current_actions->exit) current_actions->exit();
    if (new_state->init) new_state->init();
    current_actions = new_state;
}
