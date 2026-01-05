#ifndef KEYPAD_H
#define KEYPAD_H

#define ROWS 4
#define COLS 4
#define NO_KEY '\0'

/*
 * @brief Initialize keypad driver
 *
 * This function enables cols as outputs
 * Rows are input pullups
 *
 */
void keypad_init(void);

/*
 * @brief Read from keypad
 *
 * This function sets each column to 0
 * If a row is also 0, that key is pressed
 *
 * @return Pressed key, (NO_KEY if not pressed)
 */
char keypad_read(void);


#endif // KEYPAD_H
