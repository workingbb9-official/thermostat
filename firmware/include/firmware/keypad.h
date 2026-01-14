#ifndef KEYPAD_MGR_H
#define KEYPAD_MGR_H

#define NO_KEY '\0'
struct keypad_state {
    char last_key;
    char current_key;
} __attribute__((packed));

/*
 * @brief Initialize keypad
 *
 */
void keypad_mgr_init(void);

/*
 * @brief Get the keypad state
 *
 * Access struct fields to see last and current key
 * This can be used for debouncing / stop key spam
 *
 * @return Current keypad state (definition above)
 */
struct keypad_state keypad_mgr_read(void);


#endif // KEYPAD_MGR_H
