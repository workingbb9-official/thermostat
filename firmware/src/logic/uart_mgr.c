#include "logic/uart_mgr.h"

#include "drivers/uart.h"

void uart_mgr_init(void) {
    uart_init();
}

void uart_mgr_transmit(const char *data) {
	while (*data != '\0') {
        uart_transmit_c((uint8_t) *data);
		++data;
	}

    uart_transmit_c('\0');
}
