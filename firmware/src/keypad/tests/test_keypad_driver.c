#include <firmware/keypad.h>

#include <util/delay.h>
#include <firmware/lcd.h>

int main(void) {
    keypad_init();
    lcd_mgr_init();
    int was_pressed = 0;

    for (;;) {
        const char key = keypad_read();
        if (key == NO_KEY) {
            was_pressed = 0;
        } else if (!was_pressed) {
            was_pressed = 1;
            const char key_to_string[2] = {key, '\0'};
            lcd_mgr_write(key_to_string);
        }
    }

    return 0;
}
