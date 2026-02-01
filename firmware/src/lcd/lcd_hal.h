#ifndef LCD_HAL_H
#define LCD_HAL_H

#include <stdint.h>

/**
 * @brief Initialize LCD display
 *
 * This function sets all lcd pins to output and low
 * It enables 4-bit mode
 * Display on, cursor on, no blink
 */
void lcd_hal_init(void);

/**
 * @brief Send a command to the lcd
 *
 * This function clears RS before sending byte
 * Sends the high half, then the low half
 *
 * @param cmd -- Command to send
 */
void lcd_hal_send_cmd(uint8_t cmd);

/**
 * @brief Draw a byte to the screen
 *
 * This function sets RS before sending byte
 * Sends the high half, then the low half
 *
 * @param byte -- Byte to draw
 */
void lcd_hal_draw_byte(uint8_t byte);

#endif // LCD_HAL_H
