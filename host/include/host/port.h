#ifndef PORT_MGR_H
#define PORT_MGR_H

#include <thermostat/protocol.h>

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
 * @brief Send a data packet to the port
 * @param Packet to send
 *
 * @return 0 for success, -2 for syserror
 */
int port_mgr_send_packet(struct data_packet *packet);

/*
 * @brief Close the port
 * 
 * @return 0 for success, -1 for error
 */
int port_mgr_close(void);

#endif // PORT_MGR_H
