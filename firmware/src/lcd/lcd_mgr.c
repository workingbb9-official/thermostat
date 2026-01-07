#include "lcd/lcd.h"

#include "lcd_hal.h"

static void itoa(char *string, int16_t value);

void lcd_mgr_init(void) {
    lcd_init();
}

void lcd_mgr_clear(void) {
    lcd_mgr_set_cursor(0, 0);
    for (uint8_t i = 0; i < 16; ++i) {
        lcd_write_byte(' ');
    }
    lcd_mgr_set_cursor(0, 0);
}

void lcd_mgr_write(const char *string) {
    while (*string != '\0') {
        lcd_write_byte(*string);
        ++string;
    }
}

void lcd_mgr_set_cursor(uint8_t row, uint8_t col) {
    if (row > 1 || col > 15) {
        return;
    }
    static const uint8_t row_base[] = {0x00, 0x40};
    lcd_send_cmd(0x80 | (row_base[row] + col));
}

void lcd_mgr_write_int(int16_t value) {
    
    char string[8];
    itoa(string, value);

    lcd_mgr_write(string);
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
