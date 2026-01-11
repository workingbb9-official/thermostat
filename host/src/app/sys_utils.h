#ifndef SYS_UTILS_H
#define SYS_UTILS_H

struct data_packet;

/* @brief Receive data from serial port
 * @param 0-initialized Data packet
 *
 * This function is blocking
 * It will wait until invalid byte, or all bytes are read
 * Data is stored in data packet
 *
 * @return 0 for success, negative for errors
 */
int receive_data(struct data_packet *packet);

/* @brief Store temp value
 * @param Data packet with type TEMP and correct payload format
 *
 * Decrypts the value into original float
 * Then writes the data to temperature file
 *
 * @return 0 for success, negative for errors
 */
int store_temp(const struct data_packet *packet);

#endif // SYS_UTILS_H
