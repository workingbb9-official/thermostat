#ifndef KEYPAD_HAL_H
#define KEYPAD_HAL_H

#define KEYPAD_ROWS 4
#define KEYPAD_COLS 4

/**
 * @brief Initialize keypad
 *
 * This function enables cols as outputs
 * Rows are input pullups
 */
void keypad_hal_init(void);

/**
 * @brief Read a pressed key
 *
 * This function sets each column to 0
 * If a row is also 0, that key is pressed
 *
 * @return Pressed key or '\0'
 */
char keypad_hal_read(void);

#endif // KEYPAD_HAL_H
