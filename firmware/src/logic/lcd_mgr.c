#include "logic/lcd_mgr.h"

#include "drivers/lcd.h"

void lcd_mgr_init(void) {
    lcd_init();
}

void lcd_mgr_clear(void) {
    lcd_send_cmd(0x01);
}

void lcd_mgr_write(const char *string) {
    while (*string != '\0') {
        lcd_write_byte(*string);
        ++string;
    }
}
