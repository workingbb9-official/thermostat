#include "app/system_core.h"

#include <stdint.h>

#include "thermistor/thermistor.h"
#include "uart/uart.h"
#include "lcd/lcd.h"
#include "keypad/keypad.h"
#include "common/protocol.h"

#include "states.h"
#include "state_login.h"
#include "state_home.h"

static enum sys_state current_state = STATE_LOGIN;

void system_init(void) {
    therm_mgr_init();
    uart_mgr_init();
    lcd_mgr_init();
    keypad_init();
}

void system_run(void) {
    switch (current_state) {
    case STATE_LOGIN:
        login_run(&current_state);
        break;
    case STATE_HOME:
        home_run(&current_state);
        break;
    default:
        current_state = STATE_LOGIN;
        break;
    }
}
