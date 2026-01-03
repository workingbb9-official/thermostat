#include "drivers/lcd.h"

#include <stdint.h>
#include <util/delay.h>

#define DELAY 100000UL
#define CLEAR 0x01

int main(void) {
    lcd_init();
    
    const char hello[] = "Sin, sin city wasn't made for ya";
    int pos = 0;
    volatile uint64_t timer = 0;

    for (;;) {
        ++timer;
        if (timer >= DELAY) {
            lcd_write_byte(hello[pos++]);
            _delay_ms(2);
            if (hello[pos] == '\0') {
                _delay_ms(2500);
                lcd_send_cmd(CLEAR);
                pos = 0;
            }

            timer = 0;
        }
    }

    return 0;
}
