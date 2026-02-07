#ifndef PORT_HAL_H
#define PORT_HAL_H

#include <host/common/port_errors.h>
#include <sys/types.h>

/**
 * @brief Open serial port in i/o and no control mode
 *
 * @param file_path - Path of the port file
 *
 * @return File desc of port, or PORT error
 * @retval PORT_E_INVAL - Path was NULL
 * @retval PORT_E_OPEN - Call to open() failed
 */
int port_hal_open(const char *file_path);

/**
 * @brief Configure serial port
 *
 * Port data flow will use 1 stop bit, no parity, 8 bit data.
 * Configures using termios and tcgetattr().
 * Valid baud rates are: 9600, 115200.
 *
 * @param port - File desc of port
 * @param baud_rate - Transmission speed
 *
 * @retval PORT_OK - Configured port
 * @retval PORT_E_INVAL - Port was <0 or baud rate was invalid
 * @retval PORT_E_CONFIG - Call to tcgetattr() or tcsetattr() failed
 */
enum port_err port_hal_configure(int port, int baud_rate);

/**
 * @brief Read from serial port
 *
 * If successful, this function will null-terminate.
 * Otherwise, buffer is unsafe and should be discarded.
 *
 * @param port - File desc of port
 * @param buf - Buffer to store data
 * @param bytes - # of bytes to read
 *
 * @return Bytes read into buf, or PORT error
 * @retval PORT_E_INVAL - Port was <0 or buf was NULL
 * @retval PORT_E_READ - Call to read() failed
 */
ssize_t port_hal_read(int port, char *buf, size_t bytes);

/**
 * @brief Write to serial port
 *
 * @param port - File desc of port
 * @param buf - Data to write
 * @param bytes - # of bytes to write
 *
 * @return Bytes written to port, or PORT error
 * @retval PORT_E_INVAL - Port was <0 or buf was NULL
 * @retval PORT_E_WRITE - Call to write() failed
 */
ssize_t port_hal_write(int port, const char *buf, size_t bytes);

/**
 * @brief Close serial port
 *
 * @param port - File desc of port
 *
 * @retval PORT_OK - Port closed
 * @retval PORT_E_INVAL - Port was <0
 * @retval PORT_E_CLOSE - Call to close() failed
 */
enum port_err port_hal_close(int port);

#endif // PORT_HAL_H
