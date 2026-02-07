#ifndef PORT_H
#define PORT_H

#include <host/common/port_errors.h>
#include <thermostat/protocol.h>

/**
 * @brief Open and configure the serial port
 *
 * The module knows file path and holds file desc of port.
 * Settings for the port are in port_hal header.
 *
 * @retval PORT_OK - Port opened and configured
 * @retval PORT_E_OPEN - Call to port_hal_open() failed
 * @retval PORT_E_CONFIGURE - Call to port_hal_configure() failed
 */
enum port_err port_init(void);

/**
 * @brief Read a byte from the port
 *
 * @param buf - Buffer to read byte into
 *
 * @retval PORT_OK - Byte was read into buffer
 * @retval PORT_E_INVAL - Buffer was NULL
 * @retval PORT_E_READ - Call to port_hal_read() failed
 */
enum port_err port_read_byte(char *buf);

/**
 * @brief Send a data packet through the port
 *
 * @param packet - Packet to send
 *
 * @retval PORT_OK - Packet was sent
 * @retval PORT_E_INVAL - Packet was NULL
 * @retval PORT_E_WRITE - Call to port_hal_write() failed
 */
enum port_err port_send_packet(struct data_packet *packet);

/**
 * @brief Close the serial port
 *
 * @retval PORT_OK - Port closed
 * @retval PORT_E_CLOSE - Call to port_hal_close() failed
 */
enum port_err port_close(void);

#endif // PORT_H
