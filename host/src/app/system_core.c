#include <host/system_core.h>

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

#include <host/storage.h>
#include <host/analysis.h>
#include <host/port.h>
#include <host/network.h>
#include <host/weather.h>
#include <host/weather_client.h>
#include <thermostat/protocol.h>
#include "sys_utils.h"

#define API_URL "/v1/forecast?latitude=39&longitude=-97"    \
                "&current=temperature_2m&forecast_days=1"   \

static int signal_shutdown = 0;
static int signal_init(void);
static void signal_handler(int signum);

struct statistics {
    float average;
    float max;
    float min;
};

static struct statistics global_stats = {0};
static struct net_device http_dev = {0};
static struct weather_data weather = {0};

int system_init(void) {
    if (port_mgr_init() != 0) {
        return TSYS_E_PORT;
    }

    if (storage_mgr_init() != 0) {
        port_mgr_close();
        return TSYS_E_STORAGE;
    }

    if (net_dev_init(&http_dev, &http_ops, "api.open-meteo.com", API_URL)) {
        port_mgr_close();
        storage_mgr_close();
        return TSYS_E_NETWORK;
    }

    int outdoor_temp_status = weather_client_get_temp(&http_dev, &weather);
    switch (outdoor_temp_status) {
    case WEATHER_CLIENT_OK:
        printf("Outdoor temp: %.2f *C\n", weather.temp);
        break;
    case WEATHER_CLIENT_E_NET:
        printf("Network error\n");
        break;
    case WEATHER_CLIENT_E_WEATHER:
        printf("Weather error\n");
        break;
    default:
        break;
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
    
    
    int outdoor_temp_status = weather_client_get_temp(&http_dev, &weather);
    switch (outdoor_temp_status) {
    case WEATHER_CLIENT_OK:
        printf("\nOutdoor temp: %.2f *C\n", weather.temp);
        break;
    case WEATHER_CLIENT_E_NET:
        printf("Network error\n");
        break;
    case WEATHER_CLIENT_E_WEATHER:
        printf("Weather error\n");
        break;
    default:
        break;
    }

    switch (packet.type) {
    case HOME:
        store_temp(&packet);
        printf("Stored temp\n");
        if (outdoor_temp_status != WEATHER_OK)
            return;

        if (weather_client_send_weather(&weather) < 0) {
            printf("Failed to send weather\n");
            return;
        }

        printf("Sent weather\n");
        break;
    case STATS:
        if (system_analyze() < 0) {
            printf("Error analyze\n");
            break;
        }
        if (send_stats(global_stats.average, global_stats.max, global_stats.min) != 0) {
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

int system_cleanup(void) {
    int port_close_status = port_mgr_close();
    int storage_close_status = storage_mgr_close();

    if (port_close_status != 0) {
        return TSYS_E_PORT;
    }

    if (storage_close_status != 0) {
        return TSYS_E_STORAGE;
    }

    return TSYS_OK;
}

int system_analyze(void) {
    size_t capacity = 256;
    size_t count = 0;
    float *data = malloc(capacity * sizeof(float));

    if (data == NULL) {
        printf("Error malloc\n");
        return TSYS_E_ANALYZE;
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
                return TSYS_E_ANALYZE;
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

static int signal_init(void) {
    struct sigaction sa = {0};
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        return TSYS_E_SIGNAL;
    }

    return TSYS_OK;
}

static void signal_handler(int signum) {
    if (signum == SIGINT) {
        signal_shutdown = 1;
    }
}
