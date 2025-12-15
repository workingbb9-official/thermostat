#include "thermistor.h"
#include "uart.h"

#include <util/delay.h>
#include <stdlib.h>

int main() {
	therm_init();
	uart_init();

	while (1) {
        const float temp_c = therm_get_temp();

        char buff[16];
        dtostrf(temp_c, 4, 2, buff);
        uart_transmit(buff);

        _delay_ms(500);
	}

	return 0;
}
