#include <firmware/uart.h>

#include "uart_hal.h"
#include <thermostat/protocol.h>

#ifndef NULL
    #define NULL ((void *) 0)
#endif

static uint8_t validate_packet(const struct data_packet *packet);

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

struct data_packet* uart_mgr_receive(void) {
    static struct data_packet pkt = {0};
    static uint8_t stage = 0;
    static uint8_t payload_idx = 0;

    int16_t rx = uart_receive_byte();
    if (rx == -1)
        return NULL;
    
    uint8_t byte = (uint8_t) rx;

    switch (stage) {
    case 0:
        pkt.start_byte = byte;
        stage = 1;
        break;
    case 1:
        pkt.type = byte;
        stage = 2;
        break;
    case 2:
        pkt.length = byte;
        stage = 3;
        break;
    case 3:
        pkt.payload[payload_idx++] = byte;
        if (payload_idx >= pkt.length)
            stage = 4;
        break;
    case 4:
        pkt.checksum = byte;

        stage = 0;
        payload_idx = 0;

        if (validate_packet(&pkt))
            return &pkt;
        
        return NULL;
    }

    return NULL;
}

static uint8_t validate_packet(const struct data_packet *packet) {
    if (packet->start_byte != START_BYTE)
        return 0;

    if (packet->type != LOGIN &&
        packet->type != HOME  &&
        packet->type != STATS)
        return 0;

    if (packet->length <= 0 ||
        packet->length > MAX_PAYLOAD)
        return 0;

    return 1;
}
