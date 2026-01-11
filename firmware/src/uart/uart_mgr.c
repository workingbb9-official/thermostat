#include <firmware/uart.h>

#include "uart_hal.h"
#include <thermostat/protocol.h>

void uart_mgr_init(void) {
    uart_init();
}

void uart_mgr_transmit(const struct data_packet *packet) {
    uart_transmit_byte(packet->start_byte);
    uart_transmit_byte(packet->type);
    uart_transmit_byte(packet->length);

    for (uint8_t i = 0; i < packet->length; ++i) {
        uart_transmit_byte(packet->payload[i]);
    }

    uart_transmit_byte(packet->checksum);
}
