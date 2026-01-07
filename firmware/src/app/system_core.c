#include "app/system_core.h"

#include <stdint.h>

#include "app/system_temperature.h"
#include "app/system_data_handler.h"
#include "app/system_display.h"
#include "keypad/keypad.h"

#define TEMP_DELAY 1250000

static SysState current_state = LOGIN;
const char real_password[PASSWORD_LEN] = {'1', '2', '3', '4'};

static void login_loop(void);
static int8_t check_password(const char *password);
static void home_loop(void);

void system_init(void) {
    keypad_init();
    system_temperature_init();
    system_data_handler_init();
    system_display_init();
}

void system_run(void) {
    switch (current_state) {
    case LOGIN:
        login_loop();
        break;
    case HOME:
        home_loop();
        break;
    case STATS:
        break;
    default:
        current_state = LOGIN;
        break;
    }
}

static void login_loop(void) {
    static uint8_t password_pos = 0;
    static char password[PASSWORD_LEN]; 
    static char last_key;

    const char key = keypad_read();
    if (key == NO_KEY) {
        last_key = NO_KEY;
        return;
    } else if (key == last_key) {
        return;
    }
    
    last_key = key;
    password[password_pos++] = key;
    char password_terminated[password_pos - 1];
    for (uint8_t i = 0; i < password_pos; ++i) {
        password_terminated[i] = password[i];
    }

    password_terminated[password_pos] = '\0';
    system_display_password(password_terminated);

    if (password_pos >= PASSWORD_LEN) {
        if (check_password(password) == CORRECT) {
            current_state = HOME;
            system_display_password("Validated");
        } else {
            system_display_password("Invalid");
        }
        
        password_pos = 0;
        password[0] = '\0';
    }
}

static int8_t check_password(const char *password) {
    for (uint8_t i = 0; i < PASSWORD_LEN; ++i) {
        if (password[i] != real_password[i]) {
            return WRONG;
        }
    }

    return CORRECT;
}

static void home_loop(void) {
    static volatile uint32_t ticks = 0;
    ++ticks;

    if (ticks >= TEMP_DELAY) {
        const int16_t temp_int = system_get_temp();
        system_send_temp(temp_int);
        system_display_temp(temp_int);

        ticks = 0;
    }
}
