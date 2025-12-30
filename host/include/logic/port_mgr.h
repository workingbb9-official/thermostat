#ifndef PORT_MGR_H
#define PORT_MGR_H

/*
 * @brief Handles opening and config of port
 *
 * This is a self-aware module, it knows what path to use
 * Specific settings are found in port.h
 *
 * @return The fd of the port, -1 for error
 */
int port_mgr_init(void);

/*
 * @brief Reads a byte from the port
 * @param Buffer to read byte into
 *
 * @return 0 for success, -1 for invalid params, -2 for syserror
 */
int port_mgr_read_byte(char *buffer); 

/*
 * @brief Closes the port
 * 
 * Uses internal port fd for closing
 *
 * @return 0 for success, -1 for error
 */
int port_mgr_close(void);

#endif // FILE_MGR_H
