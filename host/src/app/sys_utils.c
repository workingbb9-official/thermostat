#include "sys_utils.h"

#include <stdint.h>

#include <host/storage.h>
#include <host/port.h>
#include <thermostat/protocol.h>

int receive_data(struct data_packet *packet) {
    uint8_t first_byte;
    if (port_read_byte(&first_byte) < 0) {
        return -2;
    }

    if (first_byte != START_BYTE) {
        return -1;
    }
    packet->start_byte = first_byte;

    uint8_t second_byte;
    if (port_read_byte(&second_byte) < 0) {
        return -2;
    }

    switch (second_byte) {
    case LOGIN:
        packet->type = LOGIN;
        break;
    case HOME:
        packet->type = HOME;
        break;
    case STATS:
        packet->type = STATS;
        break;
    default:
        return -1;
        break;
    }

    uint8_t third_byte;
    if (port_read_byte(&third_byte) < 0) {
        return -2;
    }

    if (third_byte < 1 || third_byte > MAX_PAYLOAD) {
        return -1;
    }
    packet->length = third_byte;

    for (uint8_t i = 0; i < packet->length; ++i) {
        uint8_t payload_byte;
        if (port_read_byte(&payload_byte) < 0) {
            return -2;
        }

        packet->payload[i] = payload_byte;
    }

    uint8_t checksum_byte;
    if (port_read_byte(&checksum_byte) < 0) {
        return -2;
    }

    packet->checksum = checksum_byte;

    return 0;
}

int store_temp(const struct data_packet *packet) {
    const int16_t value = (int16_t) (((uint16_t) packet->payload[0] << 8) | packet->payload[1]);
    const float data = value / 100.0f;
    return storage_mgr_write_temp(data);
}

int send_stats(float avg, float max, float min) {
    struct data_packet stats_packet = {0};
    stats_packet.start_byte = START_BYTE;
    stats_packet.type = STATS;
    stats_packet.length = 6;

    int16_t avg_scaled = (int16_t) (100.0f * avg);
    uint8_t avg_high = (uint8_t) (avg_scaled >> 8);
    uint8_t avg_low = (uint8_t) (avg_scaled & 0xFF);
    stats_packet.payload[0] = avg_high;
    stats_packet.payload[1] = avg_low;

    int16_t max_scaled = (int16_t) (100.0f * max);
    uint8_t max_high = (uint8_t) (max_scaled >> 8);
    uint8_t max_low = (uint8_t) (max_scaled & 0xFF);
    stats_packet.payload[2] = max_high;
    stats_packet.payload[3] = max_low;

    int16_t min_scaled = (int16_t) (100.0f * min);
    uint8_t min_high = (uint8_t) (min_scaled >> 8);
    uint8_t min_low = (uint8_t) (min_scaled & 0xFF);
    stats_packet.payload[4] = min_high;
    stats_packet.payload[5] = min_low;

    stats_packet.checksum = 6;
    if (port_send_packet(&stats_packet) < 0)
        return -1;
}
