#include "sys_utils.h"

#include <stdint.h>

#include "storage/storage.h"
#include "port/port.h"

int receive_data(DataPacket *packet) {
    uint8_t first_byte;
    if (port_mgr_read_byte(&first_byte) != 0) {
        return -2;
    }

    if (first_byte != START_BYTE) {
        return -1;
    }
    packet->start_byte = first_byte;

    uint8_t second_byte;
    if (port_mgr_read_byte(&second_byte) != 0) {
        return -2;
    }

    switch (second_byte) {
    case LOGIN:
        packet->type = LOGIN;
        break;
    case TEMP:
        packet->type = TEMP;
        break;
    case STATS:
        packet->type = STATS;
        break;
    default:
        return -1;
        break;
    }

    uint8_t third_byte;
    if (port_mgr_read_byte(&third_byte) != 0) {
        return -2;
    }

    if (third_byte < 1 || third_byte > MAX_PAYLOAD) {
        return -1;
    }
    packet->length = third_byte;
    
    for (uint8_t i = 0; i < packet->length; ++i) {
        uint8_t payload_byte;
        if (port_mgr_read_byte(&payload_byte) != 0) {
            return -2;
        }
        
        packet->payload[i] = payload_byte;
    }
    
    uint8_t checksum_byte;
    if (port_mgr_read_byte(&checksum_byte) != 0) {
        return -2;
    }

    packet->checksum = checksum_byte;

    return 0;
}

int store_temp(const DataPacket *packet) {
    const int16_t value = (int16_t) (((uint16_t) packet->payload[0] << 8) | packet->payload[1]);
    const float data = value / 100.0f;
    return storage_mgr_write_temp(data);
}
