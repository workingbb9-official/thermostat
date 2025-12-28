#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#include "services/file_utils.h"
#include "logic/analysis.h"
#include "logic/port_mgr.h"

#define BUFF_SIZE 32

int port = -1;
int data_fd = -2;

void signal_handler(int signum); 

int main(void) {
    port = port_mgr_init();
    if (port < 0) {
        printf("Error with opening port");
        return EXIT_FAILURE;
    }

    data_fd = open("host/data/temperature.txt", O_RDWR | O_APPEND);
    
    float data[256];
    int line = 0;
    while (1) {
        if (line > 256) {
            break;
        }

        char buffer[BUFF_SIZE];
        file_read_line(data_fd, buffer, sizeof(buffer), line + 1);
        if (file_read_line <= 0) {
            break;
        }

        float data_line = strtof(buffer + 1, NULL);
        printf("Data line: %f\n", data_line);
        data[line] = data_line;
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

    while (read(port, &buffer[pos], 1) == 1) {
        if (pos >= sizeof(buffer)) {
            printf("Reading was too large\n");
            if (port_mgr_close() != 0) {
                printf("Error closing port\n");
                return EXIT_FAILURE;
            }
            return EXIT_FAILURE;
        }
        if (buffer[pos] == '\0') {
            file_write_line(data_fd, buffer, strlen(buffer) + 1);
            pos = 0;
        }

        ++pos;
    }
    
    if (port_mgr_close() != 0) {
        printf("Error closing port\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

void signal_handler(int signum) { 
    if (signum == SIGINT) {
        if (port < 0 || port_mgr_close != 0) {
            printf("Error closing port\n");
            exit(EXIT_FAILURE);
        }
        if (data_fd < 0) {
            close(data_fd);
        }
        exit(EXIT_FAILURE);
    }
}
