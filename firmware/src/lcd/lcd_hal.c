#include "lcd_hal.h"

#include <avr/io.h>
#include <util/delay.h>

#include "shared/board_config.h"
#include "shared/bit_utils.h"

static void lcd_send_4_bits(uint8_t data);

void lcd_init(void) {
	_delay_ms(50); // Power-on

	// Set all lcd pins to OUTPUT
	SET_BIT(DDRD, LCD_DDR_RS);
	SET_BIT(DDRD, LCD_DDR_E);
	SET_BIT(DDRC, LCD_DDR_DP4);
	SET_BIT(DDRC, LCD_DDR_DP5);
	SET_BIT(DDRC, LCD_DDR_DP6);
	SET_BIT(DDRD, LCD_DDR_DP7);

	// Set all lcd pins to LOW
	CLR_BIT(PORTD, LCD_DP7);
	CLR_BIT(PORTD, LCD_E);
	CLR_BIT(PORTD, LCD_RS);
	CLR_BIT(PORTC, LCD_DP6);
	CLR_BIT(PORTC, LCD_DP5);
	CLR_BIT(PORTC, LCD_DP4);

	// Ensure 8-bit mode
	lcd_send_4_bits(0x30);	
	_delay_us(500);
	lcd_send_4_bits(0x30);
	_delay_us(200);
	lcd_send_4_bits(0x30);
	_delay_us(200);

    // Initialize 4-bit mode
	lcd_send_4_bits(0x20);
	_delay_ms(10);

	lcd_send_cmd(0x28); // Function set
	lcd_send_cmd(0x0E); // Display on, cursor on, no blink
	lcd_send_cmd(0x01); // Clear screen
	_delay_ms(5);
    lcd_send_cmd(0x06); // Set to move right
}

void lcd_send_cmd(uint8_t cmd) {
    CLR_BIT(PORTD, LCD_RS);

    lcd_send_4_bits(cmd & 0xF0);
    lcd_send_4_bits(cmd << 4);
}

void lcd_write_byte(uint8_t byte) {
    SET_BIT(PORTD, LCD_RS);

    lcd_send_4_bits(byte & 0xF0);
    lcd_send_4_bits(byte << 4);
}

static void lcd_send_4_bits(uint8_t data) {
    if (data & 0x80) {
        SET_BIT(PORTD, LCD_DP7);
    } else {
        CLR_BIT(PORTD, LCD_DP7);
    }

    if (data & 0x40) {
        SET_BIT(PORTC, LCD_DP6);
    } else {
        CLR_BIT(PORTC, LCD_DP6);
    }

    if (data & 0x20) {
        SET_BIT(PORTC, LCD_DP5);
    } else {
        CLR_BIT(PORTC, LCD_DP5);
    }

    if (data & 0x10) {
        SET_BIT(PORTC, LCD_DP4);
    } else {
        CLR_BIT(PORTC, LCD_DP4);
    }

    SET_BIT(PORTD, LCD_E);
    _delay_us(1);

    CLR_BIT(PORTD, LCD_E);
    _delay_us(100);
}
