#ifndef PORT_HAL_H
#define PORT_HAL_H

#include <sys/types.h>
#include <host/common/port_errors.h>

/**
 * Desc: Open serial port in i/o and no control mode
 * 
 * Params:
 *      file_path: Path of the port file
 *
 * Return:
 *      >0: File desc of the port
 *      PORT_E_INVAL: Path was NULL
 *      PORT_E_OPEN: Call to open() failed
 */
int port_hal_open(const char *file_path);

/**
 * Desc: Configure serial port
 *
 * Params:
 *      port: File desc of port
 *      baud_rate: Transmission speed
 * 
 * Return:
 *      PORT_OK: Configured port
 *      PORT_E_INVAL: Port was <0 or baud rate was invalid
 *      PORT_E_CONFIG: Call to tcgetattr() or tcsetattr() failed
 *
 * Notes:
 *      Port data flow will use 1 stop bit, no parity, 8 bit data.
 *      Configures using termios and tcgetattr().
 *      Valid baud rates are: 9600, 115200
 */
enum port_err port_hal_configure(int port, int baud_rate);

/**
 * Desc: Read from serial port
 *
 * Params:
 *      port: File desc of port
 *      buf: Buffer to store data
 *      bytes: # of bytes to read
 *
 * Return:
 *      >0: Bytes read into buf
 *      PORT_E_INVAL: Port was <0 or buf was NULL
 *      PORT_E_READ: Call to read() failed
 *
 * Notes:
 *      If successful, this function will null-terminate.
 *      Otherwise, buffer is unsafe and should be discarded.
 */
ssize_t port_hal_read(int port, char *buf, size_t bytes);

/**
 * Desc: Write to serial port
 *
 * Params:
 *      port: File desc of port
 *      buf: Data to write
 *      bytes: # of bytes to write
 *
 * Return:
 *      >0: Bytes written to port
 *      PORT_E_INVAL: Port was <0 or buf was NULL
 *      PORT_E_WRITE: Call to write() failed
 */
ssize_t port_hal_write(int port, const char *buf, size_t bytes);

/**
 * Desc: Close serial port
 *
 * Params:
 *      port: File desc of port
 *
 * Return:
 *      PORT_OK: Port closed
 *      PORT_E_INVAL: Port was <0
 *      PORT_E_CLOSE: Call to close() failed
 */
enum port_err port_hal_close(int port);

#endif // PORT_HAL_H
