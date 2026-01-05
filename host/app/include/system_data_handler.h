#ifndef SYSTEM_DATA_HANDLER_H
#define SYSTEM_DATA_HANDLER_H

typedef struct __attribute__((packed)) DataPacket DataPacket;

/*
 * @brief Take in a temperature packet and store the log
 * @param Data packet with temperature data
 * 
 * This function will reconstruct the temperature as a float
 * Then it will store it in the file from storage manager
 *
 * @return 0 for success, -1 for invalid packet, -2 for syserror
 */
int system_handle_temp(const DataPacket *packet);

#endif // SYSTEM_DATA_HANDLER_H
