#ifndef PORT_H
#define PORT_H

/**
 * @brief Opens and configures serial port
 * @param File path of port
 *
 * See implementation for configuration settings
 *
 * @return File descriptor of port (negative for failure) 
 */
int port_open(const char *file_path);

#endif // PORT_H
