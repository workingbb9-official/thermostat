#ifndef THERMISTOR_H
#define THERMISTOR_H

#include <stdint.h>

/**
 * @brief Initialize the thermistor and Analog-to-Digital converter
 *
 */
void therm_init(void);

/**
 * @brief Calculate temperature
 *
 * @return Temperature in Celsius
 */
float therm_get_temp(void);

#endif // THERMISTOR_H
