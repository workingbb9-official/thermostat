#ifndef UART_MGR_H
#define UART_MGR_H

#include <stdint.h>

/*
 * @brief Initialize UART for transmission
 *
 */
void uart_mgr_init(void);

/*
 * @brief Transmit string over UART
 * @param String to transmit
 * 
 * 
 */
void uart_mgr_transmit_string(const char *data);

#endif // UART_MGR_H
