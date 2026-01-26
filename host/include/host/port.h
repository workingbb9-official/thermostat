#ifndef PORT_H
#define PORT_H

#include <thermostat/protocol.h>
#include <host/shared/port_errors.h>

/**
 * Desc: Open and configure the serial port
 *
 * Return:
 *      PORT_OK: Port opened and configured
 *      PORT_E_OPEN: Call to port_hal_open() failed
 *      PORT_E_CONFIGURE: Call to port_hal_configure() failed
 * 
 * Notes:
 *      The module knows file path and holds file desc of port.
 *      Settings for the port are in port_hal header.
 */
enum port_err port_init(void);

/**
 * Desc: Read a byte from the port
 *
 * Params:
 *      buf: Buffer to read byte into
 *
 * Return:
 *      PORT_OK: Byte was read into buffer
 *      PORT_E_INVAL: Buffer was NULL 
 *      PORT_E_READ: Call to port_hal_read() failed
 */
enum port_err port_read_byte(char *buf); 

/**
 * Desc: Send a data packet through the port
 *
 * Params:
 *      packet: Packet to send
 *
 * Return:
 *      PORT_OK: Packet was sent
 *      PORT_E_INVAL: Packet was NULL
 *      PORT_E_WRITE: Call to port_hal_write() failed
 */
enum port_err port_send_packet(struct data_packet *packet);

/**
 * Desc: Close the serial port
 *
 * Return:
 *      PORT_OK: Port closed
 *      PORT_E_CLOSE: Call to port_hal_close() failed
 */
enum port_err port_close(void);

#endif // PORT_H
