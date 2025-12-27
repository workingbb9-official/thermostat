#include "drivers/thermistor.h"

#include <avr/io.h>

#include "board_config.h"

void therm_init(void) {
	ADMUX |= (1 << REFS0);	// Use 5v as reference voltage
	ADMUX &= ~(1 << ADLAR); // Use right-adjusted result

    // Intialize ADC (Enable, divison factor of 128)
	ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

uint16_t therm_read(void) {
	ADMUX = (ADMUX & 0xF0) | (THERM_PIN & 0x0F); // Clear lower 4 bits
	ADCSRA |= (1 << ADSC); // Start conversion

	while (ADCSRA & (1 << ADSC)); // Wait for conversion to complete
	return ADC;
}
