#include <stdint.h>
#include <avr/pgmspace.h>

#include <firmware/keypad.h>
#include <firmware/lcd.h>
#include <firmware/uart.h>
#include <thermostat/protocol.h>
#include "states.h"

#define LOGIN_PWD_LEN 4
const char pwd_actual[LOGIN_PWD_LEN + 1] PROGMEM = "1234";

enum pwd_state {
    PWD_VALID = 0,
    PWD_INVALID
} __attribute__((packed));

static struct {
    struct {
        char buf[LOGIN_PWD_LEN + 1];
        uint8_t idx;
    } user_pwd;
    char input;

    union {
        uint8_t all;
        struct {
            uint8_t lcd_dirty       : 1;
            uint8_t auth_failed     : 1;

            uint8_t input_pending   : 1;
            uint8_t tx_pending      : 1;
            uint8_t reserved        : 4;
        };
    } flags;
} login_ctx;

static void login_init(void);
static void login_keypress(void);
static void login_process(void);
static void login_display(void);

const struct state_ops state_login = {
    .init     = login_init,
    .keypress = login_keypress,
    .process  = login_process,
    .display  = login_display,
    .send     = NULL,
    .receive  = NULL,
    .exit     = NULL
};

static enum pwd_state pwd_validate(void);

static void login_init(void) {
    login_ctx.flags.all = 0;
    login_ctx.flags.lcd_dirty = 1;

    login_ctx.user_pwd.buf[0] = '\0';
    login_ctx.user_pwd.idx = 0;
}

static void login_keypress(void) {
    const struct keypad_state keypad = keypad_get_state();
    if (keypad.current_key == KEYPAD_NO_KEY ||
        keypad.current_key == keypad.last_key)
        return;

    login_ctx.flags.input_pending = 1;
    login_ctx.input = keypad.current_key;
}

static void login_process(void) {
    if (login_ctx.flags.input_pending) {
        login_ctx.flags.input_pending = 0;
        
        // Update password and null-terminate
        login_ctx.user_pwd.buf[login_ctx.user_pwd.idx] = login_ctx.input;
        login_ctx.user_pwd.buf[login_ctx.user_pwd.idx + 1] = '\0';
        ++login_ctx.user_pwd.idx;

        login_ctx.flags.lcd_dirty = 1;
    }

    if (login_ctx.user_pwd.idx != LOGIN_PWD_LEN)
        return;

    if (pwd_validate() == PWD_VALID) {
        sys_change_state(&state_home);
    } else {
        // Reset user password
        login_ctx.user_pwd.idx = 0;
        login_ctx.user_pwd.buf[0] = '\0';

        login_ctx.flags.lcd_dirty = 1;
        login_ctx.flags.auth_failed = 1;
    }
}

static void login_display(void) {
    if (!login_ctx.flags.lcd_dirty)
        return;

    login_ctx.flags.lcd_dirty = 0;

    lcd_clear();
    if (login_ctx.flags.auth_failed) {
        login_ctx.flags.auth_failed = 0;
        
        // Message for wrong password
        lcd_draw_pstr(PSTR("Invalid password"));
        lcd_set_cursor(1, 0);
        lcd_draw_pstr(PSTR("3 more tries"));
    } else {
        // Message for waiting for input
        lcd_draw_pstr(PSTR("Enter password"));
        lcd_set_cursor(1, 0);
        lcd_draw_string(login_ctx.user_pwd.buf);
    }
}

static enum pwd_state pwd_validate(void) {
    for (uint8_t i = 0; i < LOGIN_PWD_LEN; ++i) {
        if (login_ctx.user_pwd.buf[i] != pwd_actual[i])
            return PWD_INVALID;
    }

    return PWD_VALID;
}
