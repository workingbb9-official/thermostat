#ifndef LCD_H
#define LCD_H

#include <stdint.h>

/**
 * @brief Initialize LCD
 */
void lcd_init(void);

/**
 * @brief Clear the entire display
 *
 * This function will move to top left
 * Then it writes 16 spaces
 * Final position is top left (0, 0)
 */
void lcd_clear(void);

/**
 * @brief Write a string on the display
 * @param *string -- Null-terminated string
 */
void lcd_draw_string(const char *string);

/**
 * @brief Write a PSTR string on the display
 * @param *string -- PSTR("String literal")
 */
void lcd_draw_pstr(const char *string);

/**
 * @brief Clear the screen and write temp
 *
 * This will convert the int to a string
 * Then it uses lcd_draw_string()
 *
 * @param value -- Integer to draw
 */
void lcd_draw_int(int16_t value);

/**
 * @brief Set the cursor for next draw
 * @param row -- Row from 0 to 1
 * @param col -- Col from 0 to 16
 */
void lcd_set_cursor(uint8_t row, uint8_t col);

#endif // LCD_H
