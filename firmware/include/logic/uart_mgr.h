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
 * @brief Transmit string over UART
 * @param String to transmit
 * 
 */
void uart_mgr_transmit_string(const char *data);

/*
 * @brief Transmit data packet over UART
 * @param Packet to transmit
 *
 */
void uart_mgr_transmit(const DataPacket *packet);

#endif // UART_MGR_H
