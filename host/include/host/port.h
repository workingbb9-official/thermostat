#ifndef PORT_MGR_H
#define PORT_MGR_H

/*
 * @brief Open and configure port
 *
 * Module holds internal port data such as path
 * Specific settings are found in port.h
 *
 * @return 0 for success, -1 for syserror
 */
int port_mgr_init(void);

/*
 * @brief Read a byte from the port
 * @param Buffer to read byte into
 *
 * @return 0 for success, -1 for invalid params, -2 for syserror
 */
int port_mgr_read_byte(char *buffer); 

/*
 * @brief Write a byte to the port
 * @param Byte to write
 *
 * @return 0 for success, -2 for syserror
 */
int port_mgr_write_byte(char byte);

/*
 * @brief Close the port
 * 
 * @return 0 for success, -1 for error
 */
int port_mgr_close(void);

#endif // PORT_MGR_H
