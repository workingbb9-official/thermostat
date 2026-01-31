#ifndef THERMISTOR_H
#define THERMISTOR_H

/**
 * @brief Initialize thermistor
 */
void therm_init(void);

/**
 * @brief Calculate temperature
 *
 * Takes raw binary value from thermistor
 * Calculates resistance from the raw value
 * Calculates temperature using Steinhart-hart equation
 *
 * @return Current temperature in celsius
 */
float therm_get_temp(void);

#endif // THERMISTOR_H
