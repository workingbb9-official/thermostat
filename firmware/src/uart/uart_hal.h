#ifndef UART_HAL_H
#define UART_HAL_H

#include <stdint.h>

#define UART_HAL_NO_BYTE -1

/**
 * @brief Initialize UART (Baud rate in config.h, no parity, 1 stop
 * bit, 8 bit data)
 */
void uart_hal_init(void);

/**
 * @brief Transmit a byte over UART
 * @param byte - Byte to transmit
 */
void uart_hal_write_byte(uint8_t byte);

/**
 * @brief Receive a byte over UART
 *
 * All data sent to UART should be unsigned.
 * This allows for check of UART_NO_BYTE.
 *
 * @return Byte received or UART_NO_BYTE
 */
int16_t uart_hal_read_byte(void);

#endif // UART_HAL_H
