#include "drivers/lcd.h"

#include <avr/io.h>
#include <util/delay.h>

#include "drivers/bit_utils.h"

static void lcd_send_4_bits(uint8_t data);

void lcd_init(void) {
	_delay_ms(50); // Power-on

	// Set all lcd pins to OUTPUT
	SET_BIT(DDRD, DDD2);
	SET_BIT(DDRD, DDD3);
	SET_BIT(DDRD, DDD4);
	SET_BIT(DDRC, DDC3);
	SET_BIT(DDRC, DDC4);
	SET_BIT(DDRC, DDC5);

	// Set all lcd pins to LOW
	CLR_BIT(PORTD, PORTD2);
	CLR_BIT(PORTD, PORTD3);
	CLR_BIT(PORTD, PORTD4);
	CLR_BIT(PORTC, PORTC3);
	CLR_BIT(PORTC, PORTC4);
	CLR_BIT(PORTC, PORTC5);

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
    CLR_BIT(PORTD, PORTD4);

    lcd_send_4_bits(cmd & 0xF0);
    lcd_send_4_bits(cmd << 4);
}

void lcd_write_byte(uint8_t byte) {
    SET_BIT(PORTD, PORTD4);

    lcd_send_4_bits(byte & 0xF0);
    lcd_send_4_bits(byte << 4);
}

static void lcd_send_4_bits(uint8_t data) {
    if (data & 0x80) {
        SET_BIT(PORTD, PORTD2);
    } else {
        CLR_BIT(PORTD, PORTD2);
    }

    if (data & 0x40) {
        SET_BIT(PORTC, PORTC3);
    } else {
        CLR_BIT(PORTC, PORTC3);
    }

    if (data & 0x20) {
        SET_BIT(PORTC, PORTC4);
    } else {
        CLR_BIT(PORTC, PORTC4);
    }

    if (data & 0x10) {
        SET_BIT(PORTC, PORTC5);
    } else {
        CLR_BIT(PORTC, PORTC5);
    }

    SET_BIT(PORTD, PORTD3);
    _delay_us(1);

    CLR_BIT(PORTD, PORTD3);
    _delay_us(100);
}
