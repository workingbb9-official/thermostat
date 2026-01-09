#include "app/system_core.h"

#include <stdint.h>

#include "thermistor/thermistor.h"
#include "uart/uart.h"
#include "lcd/lcd.h"
#include "keypad/keypad.h"
#include "common/protocol.h"
#include "app/states.h"
#include "app/state_login.h"
#include "app/state_home.h"

static enum sys_state current_state = STATE_LOGIN;

static void home_loop(void);
static int16_t get_temp(void);
static int16_t remove_decimal(int x);
static void send_temp(int16_t temp);

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
