#ifndef SYSTEM_DATA_HANDLER_H
#define SYSTEM_DATA_HANDLER_H

#include <stdint.h>

/* 
 * @brief Initialize data handler
 *
 */
void system_data_handler_init(void);

/*
 * @brief Construct data packet and send to host
 * @param Temperature as an integer
 *
 * This function will split temp into high and low bytes
 * Payload contains a size of 2
 * Checksum not implemented yet (same as length)
 */
void system_send_temp(int16_t temp);


#endif // SYSTEM_DATA_HANDLER_H
