#include "app/system_core.h"

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

#include "storage/storage.h"
#include "analysis/analysis.h"
#include "port/port.h"
#include "common/protocol.h"

static int signal_shutdown = 0;
static ThermStatus signal_init(void);
static void signal_handler(int signum);

static int receive_data(DataPacket *packet);
static void handle_temp(const DataPacket *packet);


ThermStatus system_init(void) {
    if (port_mgr_init() != 0) {
        return TSYS_PORT_ERROR;
    }

    if (storage_mgr_init() != 0) {
        port_mgr_close();
        return TSYS_STORAGE_ERROR;
    }
    
    return signal_init();
}

void system_run(void) {
    DataPacket packet = {0};
    int receive_status = receive_data(&packet);

    if (receive_status == -1) {
        printf("Invalid packet\n");
        return;
    } else if (receive_status == -2) {
        printf("Error receive data\n");
        return;
    }

    switch (packet.type) {
    case TEMP:
        handle_temp(&packet);
        printf("Handled temp\n");
        break;
    default:
        printf("Invalid type\n");
        break;
    }
}

ThermStatus system_cleanup(void) {
    int port_close_status = port_mgr_close();
    int storage_close_status = storage_mgr_close();

    if (port_close_status != 0) {
        return TSYS_PORT_ERROR;
    }

    if (storage_close_status != 0) {
        return TSYS_STORAGE_ERROR;
    }

    return TSYS_OK;
}

ThermStatus system_analyze(void) {
    size_t capacity = 256;
    size_t count = 0;
    float *data = malloc(capacity * sizeof(float));

    if (data == NULL) {
        printf("Error malloc\n");
        return TSYS_ANALYZE_ERROR;
    }

    float temp_line;
    size_t line = 1;

    while (storage_mgr_read_temp(&temp_line, line) == 0) {
        if (count >= capacity) {
            size_t new_capacity = capacity * 2;
            float *temp_ptr = realloc(data, new_capacity * sizeof(float));

            if (temp_ptr == NULL) {
                printf("Error realloc\n");
                free(data);
                data = NULL;
                return TSYS_ANALYZE_ERROR;
            }

            capacity = new_capacity;
            data = temp_ptr;
        }

        printf("Data line: %.2f\n", temp_line);
        data[count] = temp_line;

        ++count;
        ++line;
    }

    const float average = analyze_average(data, (size_t) count);
    const float max = analyze_max(data, (size_t) count);
    const float min = analyze_min(data, (size_t) count);

    printf("\n\n\nAverage: %.2f\n", average);
    printf("Max: %.2f\n", max);
    printf("Min: %.2f\n", min);

    free(data);
    data = NULL;
    return TSYS_OK;
}

int system_should_shutdown(void) {
    return signal_shutdown;
}

static ThermStatus signal_init(void) {
    struct sigaction sa = {0};
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        return TSYS_SIGNAL_ERROR;
    }

    return TSYS_OK;
}

static void signal_handler(int signum) {
    if (signum == SIGINT) {
        signal_shutdown = 1;
    }
}

static void handle_temp(const DataPacket *packet) {
    const int16_t value = (int16_t) (((uint16_t) packet->payload[0] << 8) | packet->payload[1]);
    const float data = value / 100.0f;
    storage_mgr_write_temp(data);
}

static int receive_data(DataPacket *packet) {
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
