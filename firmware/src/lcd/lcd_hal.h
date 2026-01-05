#ifndef LCD_H
#define LCD_H

#include <stdint.h>

/*
 * @brief Initialize lcd display
 *
 * This function sets all lcd pins to output and low
 * It enables 4-bit mode
 * Display on, cursor on, no blink
 *
 */
void lcd_init(void);

/*
 * @brief Send a command to the lcd
 * @param Byte command to send
 *
 * This function clears RS before sending byte
 * Writes the full byte, then shifts left 4 and sends that
 *
 */
void lcd_send_cmd(uint8_t cmd);

/*
 * @brief Write a byte to the screen
 * @param Byte to draw on the display
 *
 * This function sets RS before sending byte
 * Writes the full byte, then shifts left 4 and sends that
 */
void lcd_write_byte(uint8_t byte);
    
#endif // LCD_H
