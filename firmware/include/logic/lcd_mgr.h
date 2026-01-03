#ifndef LCD_MGR_H
#define LCD_MGR_H

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

#endif // LCD_MGR_H
