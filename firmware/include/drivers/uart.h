#ifndef UART_H
#define UART_H

#include <stdint.h>

/**
 * @brief Initialize UART (Baud rate in config.h, no parity, 1 stop bit, 8 bit data)
 *
 */
void uart_init(void);

/**
 * @brief Transmit a char over UART
 * @param Char to transmit
 *
 */
void uart_transmit_c(uint8_t byte);

#endif // UART_H
