#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#include "logic/storage_mgr.h"
#include "logic/analysis.h"
#include "logic/port_mgr.h"

#define BUFF_SIZE 32

int data_fd = -2;

void signal_handler(int signum); 

int main(void) {
    if (port_mgr_init() < 0) {
        printf("Error with opening port");
        return EXIT_FAILURE;
    }

    if (storage_mgr_init() != 0) {
        printf("Error with storage init\n");
        port_mgr_close();
        return EXIT_FAILURE;
    }
    
    float data[256];
    int line = 0;
    while (1) {
        if (line > 256) {
            break;
        }

        float temp_line;
        if (storage_mgr_read_temp(&temp_line, line + 1) != 0) {
            break;
        }

        printf("Data line: %f\n", temp_line);
        data[line] = temp_line;
        ++line;
    }
    
    const float average = analyze_average(data, (size_t) line + 1);
    const float max = analyze_max(data, (size_t) line + 1);
    const float min = analyze_min(data, (size_t) line + 1); 

    printf("Average: %f\n", average);
    printf("Max: %f\n", max);
    printf("Min: %f\n", min);

    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    char buffer[BUFF_SIZE];
    int pos = 0;

    while (port_mgr_read_byte(&buffer[pos]) == 0) {
        if (pos >= sizeof(buffer)) {
            printf("Reading was too large\n");
            if (port_mgr_close() != 0) {
                printf("Error closing port\n");
                return EXIT_FAILURE;
            }

            if (storage_mgr_close() != 0) {
                printf("Error closing storage manager\n");
                return EXIT_FAILURE;
            }

            return EXIT_FAILURE;
        }
        if (buffer[pos] == '\0') {
            const float log = strtof(buffer + 1, NULL);
            if (storage_mgr_write_temp(log) != 0) {
                printf("Error writing temp\n");
            }

            pos = 0;
        }
        
        ++pos;
    }
    
    if (port_mgr_close() != 0) {
        printf("Error closing port\n");
        return EXIT_FAILURE;
    }

    if (storage_mgr_close() != 0) {
        printf("Error closing storage manager\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void signal_handler(int signum) { 
    if (signum == SIGINT) {
        if (port_mgr_close() != 0) {
            printf("Error closing port\n");
            exit(EXIT_FAILURE);
        }

        if (storage_mgr_close() != 0) {
            printf("Error closing storage manager\n");
            exit(EXIT_FAILURE);
        }

        exit(EXIT_SUCCESS);
    }
}
