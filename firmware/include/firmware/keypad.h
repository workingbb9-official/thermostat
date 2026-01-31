#ifndef KEYPAD_H
#define KEYPAD_H

#define KEYPAD_NO_KEY '\0'

struct keypad_state {
    char last_key;
    char current_key;
};

/**
 * @brief Initialize keypad
 */
void keypad_init(void);

/**
 * @brief Get the keypad state
 *
 * Access struct fields to see last and current key
 * This can be used for debouncing / stop key spam
 *
 * @return Current keypad state (definition above)
 */
struct keypad_state keypad_get_state(void);


#endif // KEYPAD_H
