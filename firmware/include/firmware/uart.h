#ifndef UART_MGR_H
#define UART_MGR_H

#include <stdint.h>

struct data_packet;
enum packet_status {
    INVALID_PACKET = 0,
    VALID_PACKET
};

/*
 * @brief Initialize UART for transmission
 *
 */
void uart_mgr_init(void);

/*
 * @brief Transmit data packet over UART
 * @param Packet to transmit
 * 
 * Transmit each byte of the packet
 * Loop over payload using the length provided
 *
 */
void uart_mgr_transmit(const struct data_packet *packet);

/*
 * @brief Receive data packet over UART
 * @param Packet to store received data
 *
 * This function will receive each byte
 * It will store in packet as it receives
 *
 * @return VALID_PACKET or INVALID_PACKET
 */
enum packet_status uart_mgr_receive(struct data_packet *packet_out);

#endif // UART_MGR_H
