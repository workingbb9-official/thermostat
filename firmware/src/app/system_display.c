#include "app/system_display.h"

#include "lcd/lcd.h"

static void itoa(char *string, int16_t value);

void system_display_init(void) {
    lcd_mgr_init();
}

void system_display_temp(int16_t temp) {
    
    char whole[8];
    itoa(whole, temp / 100);

    char fraction[8];
    itoa(fraction, temp % 100);

    lcd_mgr_clear();
    lcd_mgr_write("Temp: ");
    lcd_mgr_write(whole);
    lcd_mgr_write(".");
    lcd_mgr_write(fraction);
}

static void itoa(char *string, int16_t value) {
    if (value < 0) {
        *string++ = '-';
    }

    uint16_t pos_val = (value < 0) ? -value : value;

    if (pos_val == 0) {
        *string++ = '0';
    } else {
        char interim[5];
        uint8_t i = 0;
        while (pos_val > 0) {
            // Convert digit to char
            interim[i++] = '0' + (pos_val % 10);
            pos_val /= 10;
        }
        while (i > 0) {
            // Reverse to get original int
            *string++ = interim[--i];
        }
    }

    *string = '\0';
}
