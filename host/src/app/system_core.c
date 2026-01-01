#include "app/system_core.h"

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

#include "logic/storage_mgr.h"
#include "logic/analysis.h"
#include "logic/port_mgr.h"

#include"common/protocol.h"
#include "app/system_data_receiver.h"
#include "app/system_data_handler.h"

static int signal_shutdown = 0;

static void signal_init(void);
static void signal_handler(int signum);

void system_init(void) {
    if (port_mgr_init() != 0) {
        printf("Error init port manager\n");
        exit(EXIT_FAILURE);
    }

    if (storage_mgr_init() != 0) {
        printf("Error init storage manager\n");
        port_mgr_close();
        exit(EXIT_FAILURE);
    }

    signal_init();
}

void system_run(void) {
    DataPacket packet = {0};
    int receive_status = system_receive_data(&packet);

    if (receive_status == -1) {
        printf("Invalid packet\n");
        return;
    } else if (receive_status == -2) {
        printf("Error receive data\n");
        return;
    }

    switch (packet.type) {
    case TEMP:
        system_handle_temp(&packet);
        printf("Handled temp\n");
        break;
    default:
        printf("Invalid type\n");
        break;
    }
}

void system_cleanup(void) {
    int port_close_status = port_mgr_close();
    int storage_close_status = storage_mgr_close();

    if (port_close_status != 0 || storage_close_status != 0) {
        printf("Error cleanup managers\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}

void system_analyze(void) {
    size_t capacity = 256;
    size_t count = 0;
    float *data = malloc(capacity * sizeof(float));

    if (data == NULL) {
        printf("Error malloc\n");
        system_cleanup();
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
                system_cleanup();
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

    printf("Average: %f\n", average);
    printf("Max: %f\n", max);
    printf("Min: %f\n", min);

    free(data);
    data = NULL;
}

int system_should_shutdown(void) {
    return signal_shutdown;
}

static void signal_init(void) {
    struct sigaction sa = {0};
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        printf("Error init signal\n");
        exit(EXIT_FAILURE);
    }
}

static void signal_handler(int signum) {
    if (signum == SIGINT) {
        signal_shutdown = 1;
    }
}
