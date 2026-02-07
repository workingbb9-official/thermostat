#ifndef UART_H
#define UART_H

#include <stdint.h>
struct data_packet;

// UART errors run from -19 to -10
enum uart_err {
    UART_OK = 0,
    UART_INCOMPLETE = -19,
    UART_E_INVAL,
    UART_E_BAD_PACKET = -17
} __attribute__((packed));

// For receiving
struct rx_ctx {
    uint8_t stage;
    uint8_t payload_idx;
};

/**
 * Desc: Initialize UART
 */
void uart_init(void);

/**
 * Desc: Transmit data packet over UART
 *
 * Params:
 *      pkt: Packet to transmit
 */
void uart_send_packet(const struct data_packet* pkt);

/**
 * Desc: Used to clear ghost bytes from buffer
 *
 * Notes:
 *      This reads all bytes in rx buffer.
 *      The bytes are just discarded after.
 *      It will block until bytes are gone.
 *      At the end, rx buffer will be clear.
 */
void uart_clear_rx(void);

/**
 * Desc: Receive data packet over UART
 *
 * Params:
 *      ctx: Context held between calls
 *      pkt_out: Data packet to store bytes
 *
 * Notes:
 *      This function runs through 4 stages.
 *      It has start byte, type, payload, checksum.
 *      Each call moves through one stage.
 *      After receiving checksum, the packet is validated.
 *
 * Return:
 *      UART_OK: Packet is ready and valid
 *      UART_INCOMPLETE: Stages are not complete
 *      UART_E_INVAL: ctx or pkt_out are NULL
 *      UART_E_BAD_PACKET: Packet was invalid
 * @param *ctx: Context held between calls
 * @param *pkt_out: Data packet to store bytes
 * @return UART error code, UART_INCOMPLETE for unfinished packet
 */
enum uart_err uart_receive_packet(
    struct rx_ctx* ctx, struct data_packet* pkt_out);

#endif // UART_H
