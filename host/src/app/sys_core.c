#include <host/sys_core.h>

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

#include <host/port.h>
#include <host/file_utils.h>
#include <host/network.h>
#include <host/weather.h>
#include <host/common/tsys_errors.h>
#include <thermostat/protocol.h>
#include "state_login.h"
#include "state_home.h"
#include "state_stats.h"

#define API_URL "/v1/forecast?latitude=39&longitude=-97"    \
                "&current=temperature_2m&forecast_days=1"   \

static enum tsys_err signal_init(void);
static void signal_handler(int signum);
static int receive_data(struct data_packet *packet);

static struct statistics global_stats = {
    .avg = 0.0f,
    .min = 85.0f,
    .max = -50.0f
};

static struct net_device http_dev = {0};
static struct weather_data weather = {0};
static int signal_shutdown = 0;
static int temp_fd = -1;
static int login_fd = -1;

int sys_init(void) {
    if (port_init() < 0)
        return TSYS_E_PORT;

    /* Open storage files */
    temp_fd = file_open("host/data/temperature.txt");
    if (temp_fd < 0) {
        port_close();
        printf("Failed to open temp_fd\n");
        return TSYS_E_FILE;
    }
    
    login_fd = file_open("host/data/login.txt");
    if (login_fd < 0) {
        port_close();
        file_close(temp_fd);
        printf("Failed to open login_fd\n");
        return TSYS_E_FILE;
    }

    if (net_dev_init(&http_dev, &http_ops, "api.open-meteo.com", API_URL)) {
        port_close();
        file_close(temp_fd);
        return TSYS_E_NET;
    }

    return signal_init();
}

void sys_run(void) {
    struct data_packet packet = {0};
    int receive_status = receive_data(&packet);

    if (receive_status < 0) {
        printf("Invalid packet\n");
        return;
    }

    switch (packet.type) {
    case LOGIN:
        printf("Received login packet\n");
        login_record_login(login_fd);
        break;
    case LOGOUT:
        printf("Received logout packet\n");
        break;
    case HOME:
        printf("Received temp packet\n");
        if (home_store_temp(temp_fd, &packet) < 0) {
            printf("Failed to store temp\n");
        }

        if (home_send_weather(&http_dev, &weather) < 0) {
            printf("Failed to send weather\n");
        }

        break;
    case STATS:
        printf("Received stats packet\n");
        if (stats_analyze(temp_fd, &global_stats) < 0) {
            printf("Failed to analyze\n");
            global_stats.avg = 0.0f;
            global_stats.max = 0.0f;
            global_stats.min = 0.0f;
        } else {
            printf("Average: %.2f\n", global_stats.avg);
            printf("Max: %.2f\n", global_stats.max);
            printf("Min: %.2f\n", global_stats.min);
        }

        if (stats_send(&global_stats) < 0) {
            printf("Failed to send stats\n");
        }

        break;
    default:
        printf("Unexpected type\n");
        break;
    }

    printf("\n");
}

enum tsys_err sys_cleanup(void) {
    int port_close_status = port_close();
    int temp_file_close_status = file_close(temp_fd);
    int login_file_close_status = file_close(login_fd);

    if (port_close_status < 0) {
        printf("Failed to close port\n");
        return TSYS_E_PORT;
    }

    if (temp_file_close_status < 0) {
        printf("Failed to close temp file\n");
        return TSYS_E_FILE;
    }
    
    if (login_file_close_status < 0) {
        printf("Failed to close login file\n");
        return TSYS_E_FILE;
    }

    return TSYS_OK;
}

int sys_should_shutdown(void) {
    return signal_shutdown;
}

static enum tsys_err signal_init(void) {
    struct sigaction sa = {0};
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGINT, &sa, NULL) < 0)
        return TSYS_E_SIGNAL;

    return TSYS_OK;
}

static void signal_handler(int signum) {
    if (signum == SIGINT)
        signal_shutdown = 1;
}

static int receive_data(struct data_packet *packet) {
    uint8_t first_byte;
    if (port_read_byte(&first_byte) < 0)
        return TSYS_E_PORT;


    if (first_byte != START_BYTE)
        return TSYS_E_PORT;

    packet->start_byte = first_byte;

    uint8_t second_byte;
    if (port_read_byte(&second_byte) < 0)
        return TSYS_E_PORT;


    switch (second_byte) {
    case LOGIN:
        packet->type = LOGIN;
        break;
    case LOGOUT:
        packet->type = LOGOUT;
        break;
    case HOME:
        packet->type = HOME;
        break;
    case STATS:
        packet->type = STATS;
        break;
    default:
        return TSYS_E_PORT;
        break;
    }

    uint8_t third_byte;
    if (port_read_byte(&third_byte) < 0)
        return TSYS_E_PORT;


    if (third_byte < 1 || third_byte > MAX_PAYLOAD)
        return -1;

    packet->length = third_byte;

    for (uint8_t i = 0; i < packet->length; ++i) {
        uint8_t payload_byte;
        if (port_read_byte(&payload_byte) < 0)
            return TSYS_E_PORT;

        packet->payload[i] = payload_byte;
    }

    uint8_t checksum_byte;
    if (port_read_byte(&checksum_byte) < 0)
        return TSYS_E_PORT;


    packet->checksum = checksum_byte;

    return TSYS_OK;
}
