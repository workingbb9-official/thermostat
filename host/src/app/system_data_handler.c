#include "app/system_data_handler.h"

#include <stdint.h>

#include "common/protocol.h"
#include "logic/storage_mgr.h"

static inline float reconstruct_float(int16_t value);

int system_handle_temp(const DataPacket *packet) {
    if (packet->type != TEMP) {
        return -1;
    }

    const int16_t value = (int16_t) (((uint16_t) (packet->payload[0] << 8)) | ((uint16_t) packet->payload[1]));
    const float data = reconstruct_float(value);
    if (storage_mgr_write_temp(data) != 0) {
        return -2;
    }

    return 0;
}

static inline float reconstruct_float(int16_t value) {
    return value / 100.0f;
}
