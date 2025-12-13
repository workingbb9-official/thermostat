#ifndef UART_H
#define UART_H

/** 
 * @brief Initialize UART (Baud rate in config.h, no parity, 1 stop bit, 8 bit data)
 *
 */
void uart_init(void);

/**
 * @brief Transmit string over UART 
 * @param String to transmit
 *
 */
void uart_transmit(const char *data);

#endif // UART_H
