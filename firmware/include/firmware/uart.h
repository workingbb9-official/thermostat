#ifndef UART_H
#define UART_H

#include <stdint.h>
struct data_packet;

// UART errors run from -19 to -10
enum uart_err {
    UART_OK = 0,
    UART_INCOMPLETE = -19,
    UART_E_INVAL,
    UART_E_IO
} __attribute__((packed));

// For receiving
struct rx_ctx {
    uint8_t stage;
    uint8_t payload_idx;
};
    
/**
 * @brief Initialize UART
 */
void uart_init(void);

/**
 * @brief Transmit data packet over UART
 * 
 * Transmit each byte of the packet
 * Loop over payload using the length provided
 *
 * @param *pkt -- Packet to transmit
 */
void uart_send_packet(const struct data_packet *pkt);

/**
 * @brief Receive data packet over UART
 *
 * 4 stages (start byte, type, payload)
 * Each call moves through one stage
 * Validated after checksum
 *
 * @param *ctx -- Context held between calls
 * @param *pkt_out -- Data packet to store bytes
 * @return UART error code, UART_INCOMPLETE for unfinished packet
 */
enum uart_err uart_receive_packet(struct rx_ctx *ctx,
                                  struct data_packet *pkt_out);

#endif // UART_H
