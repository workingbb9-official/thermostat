#include <avr/pgmspace.h>

#include <firmware/keypad.h>
#include <firmware/lcd.h>
#include "states.h"

#define PASSWORD_LEN 4
#define FALSE 0
#define TRUE (!FALSE)

enum password_state {
    VALID = 0,
    INVALID
} __attribute__((packed));

static struct {
    const char password[PASSWORD_LEN + 1]; 
    char user_input[PASSWORD_LEN + 1];
    uint8_t pos;
    char key;
    uint8_t new_key : 1;
    uint8_t display_input : 1;
    uint8_t display_invalid : 1;
} login_data = {
    .password = "1234",
};

static void login_init(void);
static void login_keypress(enum sys_state *current_state);
static void login_process(void);
static void login_display(void);

const struct state_actions login_state = {
    .init = &login_init,
    .on_keypress = &login_keypress,
    .process = &login_process,
    .display = &login_display,
    .send = 0,
    .receive = 0,
    .exit = 0
};

static enum password_state validate_input(const char *user_input);

static void login_init(void) {
    login_data.pos = 0;
    login_data.new_key = FALSE;
    login_data.display_input = TRUE;
    login_data.display_invalid = FALSE;
}

static void login_keypress(enum sys_state *current_state) {
    const struct keypad_state keypad = keypad_mgr_read();
    if (keypad.current_key == NO_KEY ||
        keypad.current_key == keypad.last_key) {
        return;
    }

    login_data.new_key = TRUE;
    login_data.key = keypad.current_key;
}

static void login_process(void) {
    if (login_data.new_key) {
        login_data.user_input[login_data.pos] = login_data.key;
        login_data.user_input[login_data.pos + 1] = '\0';
        ++login_data.pos;
        login_data.new_key = FALSE;
        login_data.display_input = TRUE;
    }

    if (login_data.pos == PASSWORD_LEN) {
        if (validate_input(login_data.user_input) == VALID) {
            sys_change_state(&home_state);
        } else {
            login_data.pos = 0;
            login_data.display_input = FALSE;
            login_data.display_invalid = TRUE;
        }
    }
}

static void login_display(void) {
    if (login_data.display_input) {
        lcd_mgr_clear();
        lcd_mgr_write("Enter password");
        lcd_mgr_set_cursor(1, 0);
        lcd_mgr_write(login_data.user_input);
        login_data.display_input = FALSE;
    } else if (login_data.display_invalid) {
        lcd_mgr_clear();
        lcd_mgr_write("Invalid password");
        lcd_mgr_set_cursor(1, 0);
        lcd_mgr_write("3 more tries");
        login_data.display_invalid = FALSE;
    }
}


static enum password_state validate_input(const char *user_input) {
    for (uint8_t i = 0; i < PASSWORD_LEN; ++i) {
        if (user_input[i] != login_data.password[i]) {
            return INVALID;
        }
    }

    return VALID;
}
