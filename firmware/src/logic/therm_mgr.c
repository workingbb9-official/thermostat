#include "logic/therm_mgr.h"

#include <stdint.h>
#include <math.h>

#include "drivers/thermistor.h"

#define A 0.001129148
#define B 0.000234125
#define C 0.0000000876741

void therm_mgr_init(void) {
    therm_init();
}

/**
 * @brief Calculate thermistor resistance based on reading
 * @param Reading given from therm_read()
 *
 * @return float The resistance of current from thermistor in ohms
 */
static inline float calc_resistance(uint16_t reading) {
	const float ratio = (float) reading / 1023.0;
	const float resistance = 10000 * ratio / (1 - ratio);
	return resistance;
}

/**
 * @brief Calculate temperature based on resistance
 * @param Resistance given from therm_resistance()
 *
 * @return float The temperature in Celsius
 */
static float calc_temp(float resistance) {
    const float r = resistance;
    // Steinhart-hart equation to get temp from resistance
	const float temp_k = 1 / (A + B * log(r) + C * log(r) * log(r) * log(r));
	const float temp_c = temp_k - 273.15;
	return temp_c;
}

float therm_mgr_get_temp(void) {
    const uint16_t reading = therm_read();
    const float resistance = calc_resistance(reading);
    return calc_temp(resistance);
}
