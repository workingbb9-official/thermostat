#ifndef LCD_MGR_H
#define LCD_MGR_H

#include <stdint.h>
/* 
 * @brief Initialize LCD
 * 
 */
void lcd_mgr_init(void);

/*
 * @brief Clear the entire display
 *
 * This function will move to top left
 * Then it writes 16 spaces 
 * Final position is top left (0, 0)
 *
 */
void lcd_mgr_clear(void);

/*
 * @brief Write a string on the display
 * @param Null-terminated string to display
 *
 */
void lcd_mgr_write(const char *string);

/*
 * @brief Set the cursor where writing starts from
 * @param Row (0 to 1), Col (0 to 15)
 *
 */
void lcd_mgr_set_cursor(uint8_t row, uint8_t col);

/*
 * @brief Clear the screen and write temp
 * @param Integer to write onto screen
 *
 */
void lcd_mgr_write_int(int16_t value);

#endif // LCD_MGR_H
