#include "logic/uart_mgr.h"

#include "drivers/uart.h"

void uart_mgr_init(void) {
    uart_init();
}

void uart_mgr_transmit_string(const char *data) {
	while (*data != '\0') {
        uart_transmit_byte((uint8_t) *data);
		++data;
	}

    uart_transmit_byte('\0');
}
