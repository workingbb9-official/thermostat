#ifndef SYSTEM_DATA_RECEIVER_H
#define SYSTEM_DATA_RECEIVER_H

typedef struct __attribute__((packed)) DataPacket DataPacket;

/*
 * @brief Point of entry for data
 * @param Packet to store data in
 *
 * @return 0 for success, -1 for invalid packet, -2 for error
 */
int system_receive_data(DataPacket *packet);

#endif // SYSTEM_DATA_RECEIVER_H
