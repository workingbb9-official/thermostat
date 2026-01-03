#include "logic/lcd_mgr.h"

#include "drivers/lcd.h"

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
    static const uint8_t row_base[] = {0x00, 0x40};
    lcd_send_cmd(0x80 | (row_base[row] + col));
}
