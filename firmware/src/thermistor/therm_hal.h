#ifndef THERM_HAL_H
#define THERM_HAL_H

#include <stdint.h>

/**
 * @brief Initialize thermistor and ADC converter
 */
void therm_hal_init(void);

/**
 * @brief Get the raw value that thermistor stores
 *
 * @return Binary value from 0-1023
 */
uint16_t therm_hal_read(void);

#endif // THERM_HAL_H
