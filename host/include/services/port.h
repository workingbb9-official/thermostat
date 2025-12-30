#ifndef PORT_H
#define PORT_H

/**
 * @brief Opens serial port in O_RDWR mode
 * @param File path of port
 *
 * @return Fd of port, -1 for error
 */
int port_open(const char *file_path);

/**
 * @brief Configures serial port
 * @param Fd of port, transmission speed (eg. 115200)
 *
 * Uses 1 stop bit, no parity
 * @return 0 for success, -1 for invalid params, -2 for syserror
 */
int port_configure(int port, int baud_rate);

/*
 * @brief Reads bytes from given port
 * @param Fd of port, buffer to store data, # of bytes to read
 *
 * @return 0 for success, -1 for invalid params, -2 for syserror
 */
int port_read(int fd, char *buffer, int bytes);

/**
 * @brief Closes serial port
 * @param Fd of port
 *
 * @return 0 for success, -1 for error
 */
int port_close(int port);

#endif // PORT_H
