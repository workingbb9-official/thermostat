#include <host/system_core.h>

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

#include <host/storage.h>
#include <host/analysis.h>
#include <host/port.h>
#include <thermostat/protocol.h>
#include "sys_utils.h"

static int signal_shutdown = 0;
static enum therm_status signal_init(void);
static void signal_handler(int signum);

struct statistics {
    float average;
    float max;
    float min;
};

static struct statistics global_stats = {0};

enum therm_status system_init(void) {
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
    struct data_packet packet = {0};
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
        store_temp(&packet);
        printf("Handled temp\n");
        break;
    case STATS:
        if (system_analyze() != TSYS_OK) {
            printf("Error analyze\n");
            break;
        }
        if (send_stats(global_stats.average) != 0) {
            printf("Error sending stats\n");
            break;
        }
        printf("Sent stats\n");
        break;
    default:
        printf("Invalid type\n");
        break;
    }
}

enum therm_status system_cleanup(void) {
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

enum therm_status system_analyze(void) {
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

        data[count] = temp_line;

        ++count;
        ++line;
    }

    global_stats.average = analyze_average(data, (size_t) count);
    global_stats.max = analyze_max(data, (size_t) count);
    global_stats.min = analyze_min(data, (size_t) count);



    printf("Average: %.2f\n", global_stats.average);
    printf("Max: %.2f\n", global_stats.max);
    printf("Min: %.2f\n", global_stats.min);

    free(data);
    data = NULL;
    return TSYS_OK;
}

int system_should_shutdown(void) {
    return signal_shutdown;
}

static enum therm_status signal_init(void) {
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
