#ifndef UART_H
#define UART_H

#include <stdint.h>

/**
 * @brief Initialize UART (Baud rate in config.h, no parity, 1 stop bit, 8 bit data)
 *
 */
void uart_init(void);

/**
 * @brief Transmit a byte over UART
 * @param Byte to transmit
 *
 */
void uart_transmit_byte(uint8_t byte);

/**
 * @brief Receive a byte over UART
 *
 *@return Byte received
 */
 uint8_t uart_receive_byte(void);

#endif // UART_H
