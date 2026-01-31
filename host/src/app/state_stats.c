#include "state_stats.h"

#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>

#include <host/analysis.h>
#include <host/file_utils.h>
#include <host/port.h>
#include <thermostat/protocol.h>
#include <host/common/tsys_errors.h>

enum tsys_err stats_analyze(int temp_fd, struct statistics *stats_out) {
    float value = 0.0f;
    size_t count = 0;
    char buf[16];

    // Read from start
    if (file_seek(temp_fd, START) < 0) {
        return TSYS_E_FILE;
    }

    // Read each line using seeker semantics
    while (file_read_line(temp_fd, buf, sizeof(buf)) > 0) {
        value = strtof(buf, NULL);
        if (value > stats_out->max) {
            // Found our new max
            stats_out->max = value;
        } else if (value < stats_out->min) {
            // Found our new min
            stats_out->min = value;
        }

        stats_out->avg += (value - stats_out->avg) / (count + 1);
    }

    return TSYS_OK;
}

enum tsys_err stats_send(const struct statistics *stats) {
    if (!stats) {
        return TSYS_E_INVAL;
    }

    struct data_packet pkt = {0};
    pkt.start_byte = START_BYTE;
    pkt.type = STATS;
    pkt.length = 6;
    
    // Format average
    int16_t avg_scaled = (int16_t) (100.0f * stats->avg);
    uint8_t avg_high = (uint8_t) (avg_scaled >> 8);
    uint8_t avg_low = (uint8_t) (avg_scaled & 0xFF);
    pkt.payload[0] = avg_high;
    pkt.payload[1] = avg_low;
    
    // Format max
    int16_t max_scaled = (int16_t) (100.0f * stats->max);
    uint8_t max_high = (uint8_t) (max_scaled >> 8);
    uint8_t max_low = (uint8_t) (max_scaled & 0xFF);
    pkt.payload[2] = max_high;
    pkt.payload[3] = max_low;
    
    // Format min
    int16_t min_scaled = (int16_t) (100.0f * stats->min);
    uint8_t min_high = (uint8_t) (min_scaled >> 8);
    uint8_t min_low = (uint8_t) (min_scaled & 0xFF);
    pkt.payload[4] = min_high;
    pkt.payload[5] = min_low;

    pkt.checksum = 6;
    if (port_send_packet(&pkt) < 0) {
        return TSYS_E_PORT;
    }

    return TSYS_OK;
}
