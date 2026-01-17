#include <stdint.h>
#include <avr/pgmspace.h>

#include <firmware/keypad.h>
#include <firmware/lcd.h>
#include "states.h"

#define PASSWORD_LEN 4
const char password[PASSWORD_LEN + 1] PROGMEM = "1234";

enum password_state {
    PASS_VALID = 0,
    PASS_INVALID
} __attribute__((packed));

static struct {
    char            user_input[PASSWORD_LEN + 1];
    uint8_t         pos;
    char            key;
    struct {
        uint8_t     key_pending     : 1;
        uint8_t     lcd_dirty       : 1;
        uint8_t     auth_failed     : 1;
    } flags;
} login_data;

static void login_init(void);
static void login_keypress(void);
static void login_process(void);
static void login_display(void);

const struct state_actions login_state = {
    .init           = login_init,
    .on_keypress    = login_keypress,
    .process        = login_process,
    .display        = login_display,
    .send           = 0,
    .receive        = 0,
    .exit           = 0
};

static enum password_state validate_input(const char *user_input);

static void login_init(void) {
    login_data.pos = 0;
    login_data.flags.key_pending = 0;
    login_data.flags.lcd_dirty = 1;
    login_data.flags.auth_failed = 0;
}

static void login_keypress(void) {
    const struct keypad_state keypad = keypad_mgr_read();
    if (keypad.current_key == NO_KEY ||
        keypad.current_key == keypad.last_key)
        return;

    login_data.key = keypad.current_key;
    login_data.flags.key_pending = 1;
}

static void login_process(void) {
    if (login_data.flags.key_pending) {
        login_data.flags.key_pending = 0;
        login_data.user_input[login_data.pos] = login_data.key;
        login_data.user_input[login_data.pos + 1] = '\0';
        ++login_data.pos;
        login_data.flags.lcd_dirty = 1;
    }

    if (login_data.pos == PASSWORD_LEN) {
        if (validate_input(login_data.user_input) == PASS_VALID) {
            sys_change_state(&home_state);
        } else {
            login_data.pos = 0;
            login_data.flags.lcd_dirty = 1;
            login_data.flags.auth_failed = 1;
        }
    }
}

static void login_display(void) {
    if (!login_data.flags.lcd_dirty)
        return;

    login_data.flags.lcd_dirty = 0;
    lcd_mgr_clear();

    if (login_data.flags.auth_failed) {
        login_data.flags.auth_failed = 0;
        lcd_mgr_write("Invalid password");
        lcd_mgr_set_cursor(1, 0);
        lcd_mgr_write("3 more tries");
    } else {
        lcd_mgr_write("Enter password");
        lcd_mgr_set_cursor(1, 0);
        lcd_mgr_write(login_data.user_input);
    }
}


static enum password_state validate_input(const char *user_input) {
    for (uint8_t i = 0; i < PASSWORD_LEN; ++i) {
        if (user_input[i] != password[i]) {
            return PASS_INVALID;
        }
    }

    return PASS_VALID;
}
