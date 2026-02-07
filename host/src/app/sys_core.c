#include <host/sys_core.h>

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include <host/common/tsys_errors.h>
#include <host/file_utils.h>
#include <host/network.h>
#include <host/port.h>
#include <host/weather.h>
#include <thermostat/protocol.h>

#include "session.h"
#include "state_home.h"
#include "state_stats.h"

#define API_URL                                                      \
    "/v1/forecast?latitude=39&longitude=-97"                         \
    "&current=temperature_2m,weather_code"                           \
    "&forecast_days=1"

static struct statistics global_stats = {
    .avg = 0.0f,
    .min = 85.0f,
    .max = -50.0f,
};

static struct net_device http_dev = {0};
static struct weather_data weather = {0};
static int signal_shutdown = 0;
static int temp_fd = -1;
static int session_fd = -1;
static int pwd_fd = -1;

static void signal_handler(int signum)
{
    if (signum == SIGINT) {
        signal_shutdown = 1;
    }
}

static enum tsys_err signal_init(void)
{
    struct sigaction sa = {0};
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGINT, &sa, NULL) < 0) {
        return TSYS_E_SIGNAL;
    }

    return TSYS_OK;
}

static int receive_data(struct data_packet *packet)
{
    uint8_t first_byte;
    if (port_read_byte(&first_byte) < 0) {
        return TSYS_E_PORT;
    }

    if (first_byte != START_BYTE) {
        return TSYS_E_PORT;
    }

    packet->start_byte = first_byte;

    uint8_t second_byte;
    if (port_read_byte(&second_byte) < 0) {
        return TSYS_E_PORT;
    }

    switch (second_byte) {
    case LOGIN:
        packet->type = LOGIN;
        break;
    case LOGOUT:
        packet->type = LOGOUT;
        break;
    case TEMP:
        packet->type = TEMP;
        break;
    case STATS:
        packet->type = STATS;
        break;
    default:
        return TSYS_E_PORT;
        break;
    }

    uint8_t third_byte;
    if (port_read_byte(&third_byte) < 0) {
        return TSYS_E_PORT;
    }

    if (third_byte < 1 || third_byte > MAX_PAYLOAD) {
        return -1;
    }

    packet->length = third_byte;

    for (uint8_t i = 0; i < packet->length; ++i) {
        uint8_t payload_byte;
        if (port_read_byte(&payload_byte) < 0) {
            return TSYS_E_PORT;
        }

        packet->payload[i] = payload_byte;
    }

    uint8_t checksum_byte;
    if (port_read_byte(&checksum_byte) < 0) {
        return TSYS_E_PORT;
    }

    packet->checksum = checksum_byte;

    return TSYS_OK;
}

static void handle_login_request(const char *pwd)
{
    int ret;

    // TODO: Implement all session_*_pwd functions
    int output;
    ret = session_validate_pwd(pwd_fd, pwd, &output);
    if (ret < 0) {
        printf("Failed to validate pwd\n");
        return;
    }

    if (output == SESSION_PWD_INVALID) {
        ret = session_send_invalid_pwd();
        if (ret < 0) {
            printf("Failed to send invalid pwd\n");
        }
        return;
    }

    ret = session_send_valid_pwd();
    if (ret < 0) {
        printf("Failed to send valid pwd\n");
    }
}

int sys_init(void)
{
    int ret;

    ret = port_init();
    if (ret < 0) {
        return TSYS_E_PORT;
    }

    /* Open storage files */
    ret = file_open("host/data/temperature.txt");
    if (ret < 0) {
        printf("Failed to open temp_fd\n");
        ret = TSYS_E_FILE;
        goto err_temp;
    }
    temp_fd = ret;

    ret = file_open("host/data/session.txt");
    if (ret < 0) {
        printf("Failed to open session_fd\n");
        ret = TSYS_E_FILE;
        goto err_session;
    }
    session_fd = ret;

    ret = file_open("host/data/password.txt");
    if (ret < 0) {
        printf("Failed to open pwd_fd\n");
        ret = TSYS_E_FILE;
        goto err_pwd;
    }

    if (net_dev_init(
            &http_dev,
            &http_ops,
            "api.open-meteo.com",
            API_URL)) {
        printf("Failed to init net device\n");
        ret = TSYS_E_NET;
        goto err_net;
    }

    ret = signal_init();
    return ret;

err_net:
    file_close(pwd_fd);
err_pwd:
    file_close(session_fd);
err_session:
    file_close(temp_fd);
err_temp:
    return ret;
}

void sys_run(void)
{
    struct data_packet packet = {0};
    int receive_status = receive_data(&packet);

    if (receive_status < 0) {
        printf("Invalid packet\n");
        return;
    }

    switch (packet.type) {
    case AUTH:
        // TODO: Have firmware send password
        // For now we fake a valid password
        handle_login_request("1234");
    case LOGIN:
        printf("Received login packet\n");
        session_record_login(session_fd);
        break;
    case LOGOUT:
        printf("Received logout packet\n");
        session_record_logout(session_fd);
        break;
    case TEMP:
        printf("Received temp packet\n");
        if (home_store_temp(temp_fd, &packet) < 0) {
            printf("Failed to store temp\n");
        }

        int send_weather_err = home_send_weather(&http_dev, &weather);
        if (send_weather_err < 0) {
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

enum tsys_err sys_cleanup(void)
{
    int port_close_status = port_close();
    int temp_file_close_status = file_close(temp_fd);
    int session_file_close_status = file_close(session_fd);
    int pwd_file_close_status = file_close(pwd_fd);

    if (port_close_status < 0) {
        printf("Failed to close port\n");
        return TSYS_E_PORT;
    }

    if (temp_file_close_status < 0) {
        printf("Failed to close temp file\n");
        return TSYS_E_FILE;
    }

    if (session_file_close_status < 0) {
        printf("Failed to close session file\n");
        return TSYS_E_FILE;
    }

    if (pwd_file_close_status < 0) {
        printf("Failed to close password file\n");
        return TSYS_E_FILE;
    }

    return TSYS_OK;
}

int sys_should_shutdown(void)
{
    return signal_shutdown;
}
