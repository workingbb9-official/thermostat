#ifndef PORT_MGR_H
#define PORT_MGR_H

/*
 * @brief Handles opening and config of port
 * @param Path of port
 *
 * Specific settings are found in port.h
 *
 * @return The fd of the port, negative for errors
 */
int port_mgr_init(const char *port_path);





#endif // FILE_MGR_H
