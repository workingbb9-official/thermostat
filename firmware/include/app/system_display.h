#ifndef SYSTEM_DISPLAY_H
#define SYSTEM_DISPLAY_H

#include <stdint.h>

/*
 * @brief Initialize display system
 *
 */
void system_display_init(void);

/*
 * @brief Display temperature
 * @param Temperature as an integer
 * 
 * Converts integer to string then writes
 * Display is in the form Temp: xx.xx
 * Starts from the top left corner
 *
 */
void system_display_temp(int16_t temp);

#endif // SYSTEM_DISPLAY_H
