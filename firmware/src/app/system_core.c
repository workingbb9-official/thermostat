#include <firmware/system_core.h>

#include <firmware/thermistor.h>
#include <firmware/uart.h>
#include <firmware/lcd.h>
#include <firmware/keypad.h>
#include "states.h"
#include "state_login.h"
#include "state_home.h"
#include "state_stats.h"

static enum sys_state current_state = STATE_LOGIN;

void system_init(void) {
    therm_mgr_init();
    uart_mgr_init();
    lcd_mgr_init();
    keypad_mgr_init();
}

void system_run(void) {
    switch (current_state) {
    case STATE_LOGIN:
        login_run(&current_state);
        break;
    case STATE_HOME:
        home_state.process();
        home_state.on_keypress(&current_state);
        home_state.display();
        home_state.send();
        break;
    case STATE_STATS:
        stats_run(&current_state);
        break;
    default:
        current_state = STATE_LOGIN;
        break;
    }
}
