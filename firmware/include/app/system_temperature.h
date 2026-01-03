#ifndef SYSTEM_TEMPERATURE_H
#define SYSTEM_TEMPERATURE_H

#include <stdint.h>

/* 
 * @brief Initialize temperature system
 *
 */
void system_temperature_init(void);

/*
 * @brief Get temperature from thermistor
 * 
 * Shifts the float left 2 and rounds
 * This allows for handling as integer
 *
 * @return Temperature in celsius as an integer
 */
int16_t system_get_temp(void);

#endif // SYSTEM_TEMPERATURE_T
