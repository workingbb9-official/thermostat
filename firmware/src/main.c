#include <util/delay.h>
#include <stdlib.h>

#include "logic/therm_mgr.h"
#include "drivers/uart.h"

int main() {
	therm_mgr_init();
	uart_init();

	while (1) {
        const float temp_c = therm_mgr_get_temp();

        char buff[16];
        dtostrf(temp_c, 4, 2, buff);
        uart_transmit(buff);

        _delay_ms(3000);
	}

	return EXIT_SUCCESS;
}
