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

enum packet_status uart_mgr_receive(struct data_packet *packet_out) {
    uint8_t first_byte = uart_receive_byte();
    if (first_byte != START_BYTE) {
        return INVALID_PACKET;
    }
    packet_out->start_byte = first_byte;

    uint8_t second_byte = uart_receive_byte();
    if (second_byte != LOGIN &&
        second_byte != TEMP &&
        second_byte != STATS) {
        return INVALID_PACKET;
    }
    packet_out->type = second_byte;

    uint8_t third_byte = uart_receive_byte();
    if (third_byte < 1 || third_byte > MAX_PAYLOAD) {
        return INVALID_PACKET;
    }
    packet_out->length = third_byte;

    for (uint8_t i = 0; i < packet_out->length; ++i) {
        uint8_t payload_byte = uart_receive_byte();
        packet_out->payload[i] = payload_byte;
    }

    uint8_t checksum_byte = uart_receive_byte();
    packet_out->checksum = checksum_byte;

    return VALID_PACKET;
}
