#ifndef PORT_H
#define PORT_H

/**
 * @brief Opens serial port in O_RDWR mode
 * @param File path of port
 *
 * @return File descriptor of port (negative for failure) 
 */
int port_open(const char *file_path);

/**
 * @brief Configures serial port
 * @param Fd of port, transmission speed (eg. 115200)
 *
 * Uses /dev/ttyACM0, 1 stop bit, no parity
 * @return Error code (0 for success, 1 for failure)
 */
int port_configure(int port, int baud_rate);

/**
 * @brief Closes serial port
 * @param Fd of port
 *
 * @return Error code (0 for success, 1 for failure)
 */
int port_close(int port);

#endif // PORT_H
