#ifndef PORT_MGR_H
#define PORT_MGR_H

/*
 * @brief Handles opening and config of port
 *
 * This is a self-aware module, it knows what path to use
 * Specific settings are found in port.h
 *
 * @return The fd of the port, negative for errors
 */
int port_mgr_init(void);

/*
 * @brief Closes the port
 * 
 * Uses internal port fd for closing
 *
 * @return Error code (0 for success, 1 for error)
 */
int port_mgr_close(void);





#endif // FILE_MGR_H
