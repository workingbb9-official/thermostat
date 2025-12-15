#ifndef PORT_H
#define PORT_H

/**
 * @brief Configures serial port
 * @param Id of port from open(), transmission speed (eg. 115200)
 *
 * Uses /dev/ttyACM0, 1 stop bit, no parity
 * @return Error code (0 for success, 1 for failure)
 */
int port_configure(int port, int baud_rate);

/**
 * @brief Reads from serial port and converts to ASCII
 * @param Id of port from open()
 *
 * @return ASCII representation of binary from port
 */
char* port_read(int port);
#endif // PORT_H
