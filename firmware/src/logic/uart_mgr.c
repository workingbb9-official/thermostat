#include "logic/uart_mgr.h"

#include "drivers/uart.h"
#include "common/protocol.h"

void uart_mgr_init(void) {
    uart_init();
}

/* void uart_mgr_transmit_string(const char *data) {
	while (*data != '\0') {
        uart_transmit_byte((uint8_t) *data);
		++data;
	}

    uart_transmit_byte('\0');
} */

void uart_mgr_transmit(const DataPacket *packet) {
    uart_transmit_byte(packet->start_byte);
    uart_transmit_byte(packet->type);
    uart_transmit_byte(packet->length);

    for (uint8_t i = 0; i < packet->length; ++i) {
        uart_transmit_byte(packet->payload[i]);
    }

    uart_transmit_byte(packet->checksum);
}
