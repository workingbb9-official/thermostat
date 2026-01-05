#ifndef UART_MGR_H
#define UART_MGR_H

#include <stdint.h>

typedef struct __attribute__((packed)) DataPacket DataPacket;

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
void uart_mgr_transmit(const DataPacket *packet);

#endif // UART_MGR_H
