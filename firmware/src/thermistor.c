#include "thermistor.h"

#include <avr/io.h>
#include <math.h>

#define A 0.001129148
#define B 0.000234125
#define C 0.0000000876741

void therm_init(void) {
	ADMUX |= (1 << REFS0);	// Use 5v as reference voltage
	ADMUX &= ~(1 << ADLAR); // Use right-adjusted result

    // Intialize ADC (Enable, divison factor of 128)
	ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

/**
 * @brief Gets the value that thermistor stores
 *
 * @return uint16_t A binary value from 0-1023
 */
static uint16_t therm_read(void) {
	ADMUX = (ADMUX & 0xF0) | (0 & 0x0F); // Clear lower 4 bits
	ADCSRA |= (1 << ADSC); // Start conversion

	while (ADCSRA & (1 << ADSC)); // Wait for conversion to complete
	return ADC;
}

/**
 * @brief Calculate thermistor resistance based on reading
 * @param Reading given from therm_read()
 *
 * @return float The resistance of current from thermistor in ohms
 */
static inline float therm_calc_resistance(const uint16_t reading) {
    // Calculate thermistor resistance from its reading
	const float ratio = (float)reading / 1023.0;
	const float resistance = 10000 * ratio / (1 - ratio);
	return resistance;
}

/**
 * @brief Calculate temperature based on resistance
 * @param Resistance given from therm_resistance()
 *
 * @return float The temperature in Celsius
 */
static float therm_calc_temp(float resistance) {
    const float r = resistance;
    // Steinhart-hart equation to get temp from resistance
	const float temp_k = 1 / (A + B * log(r) + C * log(r) * log(r) * log(r));
	const float temp_c = temp_k - 273.15;
	return temp_c;
}

float therm_get_temp(void) {
    const uint16_t reading = therm_read();
    const float resistance = therm_calc_resistance(reading);
    return therm_calc_temp(resistance);
}
