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
    const int receive_status = system_receive_data(&packet);
    if (receive_status == -1) {
        printf("Invalid packet\n");
    } else if (receive_status == -2) {
        printf("Error receive data\n");
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
    const int port_close_status = port_mgr_close();
    const int storage_close_status = storage_mgr_close();

    if (port_close_status != 0 || storage_close_status != 0) {
        printf("Error cleanup managers\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}

void system_analyze(void) {
    float data[256];
    int line = 1;

    while (storage_mgr_read_temp(&temp_line, line) != 0) {
        if (line - 1 > 256) {
            break;
        }

        float temp_line;

        printf("Data line: %.2f\n", temp_line);
        data[line - 1] = temp_line;
        ++line;
    }

    const float average = analyze_average(data, (size_t) line - 1);
    const float max = analyze_max(data, (size_t) line - 1);
    const float min = analyze_min(data, (size_t) line - 1);

    printf("Average: %f\n", average);
    printf("Max: %f\n", max);
    printf("Min: %f\n", min);
}


static void signal_init(void) {
    struct sigaction sa;
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
        system_cleanup();
    }
}
