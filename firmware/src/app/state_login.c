#include <stdint.h>
#include <avr/pgmspace.h>

#include <firmware/keypad.h>
#include <firmware/lcd.h>
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
    } pwd;
    char input;

    union {
        uint8_t all;
        struct {
            uint8_t lcd_dirty   : 1;
            uint8_t input_pending : 1;
            uint8_t auth_failed : 1;
            uint8_t reserved    : 5;
        }
    } flags;
} login_ctx;

static void login_init(void);
static void login_keypress(void);
static void login_process(void);
static void login_display(void);

const struct state_actions login_state = {
    .init        = login_init,
    .on_keypress = login_keypress,
    .process     = login_process,
    .display     = login_display,
    .send        = 0,
    .receive     = 0,
    .exit        = 0
};

static enum pwd_state pwd_validate(void);

static void login_init(void) {
    login_ctx.flags.all = 0;
    login_ctx.pwd.buf[0] = '\0';
    login_ctx.pwd.idx = 0;
}

static void login_keypress(void) {
    const struct keypad_state keypad = keypad_mgr_read();
    if (keypad.current_key == NO_KEY ||
        keypad.current_key == keypad.last_key)
        return;

    login_ctx.flags.input_pending = 1;
    login_ctx.input = keypad.current_key;
}

static void login_process(void) {
    if (login_ctx.flags.input_pending) {

        login_ctx.flags.input_pending = 0;

        login_ctx.pwd.buf[login_ctx.pwd.idx] = login_ctx.input;
        login_ctx.pwd.buf[login_ctx.pwd.idx + 1] = login_ctx.input;
        ++login_ctx.pwd.idx;

        login_ctx.flags.lcd_dirty = 1;
    }

    if (login_ctx.pwd.idx != LOGIN_PWD_LEN)
        return;

    if (pwd_validate() == PWD_VALID) {
        sys_change_state(&home_state);
    } else {

        login_ctx.pwd.idx = 0;

        login_ctx.flags.lcd_dirty = 1;
        login_ctx.flags.auth_failed = 1;
    }
}

static void login_display(void) {
    if (!login_ctx.flags.lcd_dirty)
        return;

    login_ctx.flags.lcd_dirty = 0;

    lcd_mgr_clear();
    if (login_ctx.flags.auth_failed) {

        login_ctx.flags.auth_failed = 0;

        lcd_mgr_write("Invalid password");
        lcd_mgr_set_cursor(1, 0);
        lcd_mgr_write("3 more tries");
    } else {
        lcd_mgr_write("Enter password");
        lcd_mgr_set_cursor(1, 0);
        lcd_mgr_write(login_ctx.pwd.buf);
    }
}


static enum pwd_state pwd_validate(void) {
    for (uint8_t i = 0; i < LOGIN_PWD_LEN; ++i) {
        if (login_ctx.pwd.buf[i] != pwd_actual[i])
            return PWD_INVALID;
    }

    return PWD_VALID;
}
