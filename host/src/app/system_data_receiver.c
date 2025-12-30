#include "app/system_data_receiver.h"

#include <stdint.h>
#include <stddef.h>

#include "common/protocol.h"
#include "logic/port_mgr.h"

static int calculate_checksum(const uint8_t *payload, uint8_t len);

int system_receive_data(DataPacket *packet) {
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

    if (calculate_checksum(packet->payload, checksum_byte) != 0) {
        return -1;
    }
    packet->checksum = checksum_byte;

    return 0;
}

static int calculate_checksum(const uint8_t *payload, uint8_t len) {
    size_t size = sizeof(payload) / sizeof(payload[0]);
    if (size != (size_t) len) {
        return -1;
    }

    return 0;
}
