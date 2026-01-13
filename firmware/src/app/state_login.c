#include "state_login.h"

#include <firmware/keypad.h>
#include <firmware/lcd.h>
#include "states.h"

enum password_state {
    VALID = 0
    INVALID
} __attribute__((packed));

const char PASSWORD[PASSWORD_LEN] = "1234";
static enum password_state validate_input(const char *user_input);

void login_run(enum sys_state *current_state) {
    static char user_input[PASSWORD_LEN] = {0};
    static int pos = 0;
    static char last_key = NO_KEY;

    const char key = keypad_read();
    if (key == NO_KEY || key == last_key) {
        return;
    }

    // Store key and terminate string
    user_input[pos] = key;
    user_input[pos + 1] = '\0';
    ++pos;
    last_key = key;

    lcd_mgr_clear();
    if (pos != PASSWORD_LEN) {
        lcd_mgr_write(user_input);
        return;
    }

    if (validate_input(user_input) == VALID) {
        lcd_mgr_write("Logging you in");
        pos = 0;
        *current_state = STATE_HOME;
    } else {
        lcd_mgr_write("Invalid password");
        pos = 0;
    }
}

static enum password_state validate_input(const char *user_input) {
    for (uint8_t i = 0; i < PASSWORD_LEN; ++i) {
        if (user_input[i] != PASSWORD[i]) {
            return INVALID;
        }
    }

    return VALID;
}
