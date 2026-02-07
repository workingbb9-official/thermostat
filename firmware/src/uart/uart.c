#include <firmware/uart.h>

#include <thermostat/protocol.h>

#include "uart_hal.h"

#ifndef NULL
#define NULL ((void *) 0)
#endif

static uint8_t validate_packet(const struct data_packet *pkt)
{
    if (pkt->start_byte != START_BYTE) {
        return 0;
    }

    if (pkt->type != WEATHER && pkt->type != STATS) {
        return 0;
    }

    if (pkt->length <= 0 || pkt->length > MAX_PAYLOAD) {
        return 0;
    }

    return 1;
}

void uart_init(void)
{
    uart_hal_init();
}

void uart_send_packet(const struct data_packet *pkt)
{
    uart_hal_write_byte(pkt->start_byte);
    uart_hal_write_byte(pkt->type);
    uart_hal_write_byte(pkt->length);

    for (uint8_t i = 0; i < pkt->length; ++i) {
        uart_hal_write_byte(pkt->payload[i]);
    }

    uart_hal_write_byte(pkt->checksum);
}

void uart_clear_rx(void)
{
    while (uart_hal_read_byte() != UART_HAL_NO_BYTE) {
        // Read until no more bytes in buffer
    }
}

enum uart_err uart_receive_packet(
    struct rx_ctx *ctx,
    struct data_packet *pkt_out)
{
    if (!ctx || !pkt_out) {
        return UART_E_INVAL;
    }

    int16_t rx = uart_hal_read_byte();
    if (rx == UART_HAL_NO_BYTE) {
        return UART_INCOMPLETE;
    }

    uint8_t byte = (uint8_t) rx;

    // Stage represents each member of data packet struct
    switch (ctx->stage) {
    case 0:
        if (byte == START_BYTE) {
            pkt_out->start_byte = byte;
            ctx->stage = 1;
        }

        break;

    case 1:
        pkt_out->type = byte;
        ctx->stage = 2;
        break;

    case 2:
        pkt_out->length = byte;
        ctx->stage = 3;
        break;

    case 3:
        pkt_out->payload[ctx->payload_idx++] = byte;
        if (ctx->payload_idx >= pkt_out->length) {
            ctx->stage = 4;
        }
        break;

    case 4:
        pkt_out->checksum = byte;

        ctx->stage = 0;
        ctx->payload_idx = 0;

        if (validate_packet(pkt_out)) {
            return UART_OK;
        }

        return UART_E_BAD_PACKET;

    default:
        // Reset ctx
        ctx->stage = 0;
        ctx->payload_idx = 0;
        break;
    }

    return UART_INCOMPLETE;
}
