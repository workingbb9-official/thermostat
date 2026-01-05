#ifndef THERMISTOR_H
#define THERMISTOR_H

#include <stdint.h>

/**
 * @brief Initialize the thermistor and Analog-to-Digital converter
 *
 */
void therm_init(void);

/**
 * @brief Get the raw value that thermistor stores
 *
 * @return A binary value from 0-1023
 */
uint16_t therm_read(void);

#endif // THERMISTOR_H
