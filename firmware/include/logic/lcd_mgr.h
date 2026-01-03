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

#endif // LCD_MGR_H
