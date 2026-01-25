#include "therm_hal.h"

#include <avr/io.h>

#include <firmware/common/board_config.h>
#include <firmware/common/bit_utils.h>

void therm_hal_init(void) {
    SET_BIT(ADMUX, REFS0); // Use 5v as reference voltage
    CLR_BIT(ADMUX, ADLAR); // Use right-adjusted result

    // Intialize ADC (Enable, divison factor of 128)
    ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

uint16_t therm_hal_read(void) {
    ADMUX = (ADMUX & 0xF0) | (THERM_PIN & 0x0F); // Clear lower 4 bits
    SET_BIT(ADCSRA, ADSC);// Start conversion

    while (ADCSRA & (1 << ADSC)); // Wait for conversion to complete
    return ADC;
}
