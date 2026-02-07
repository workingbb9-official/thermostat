#include "state_home.h"

#include <stdint.h>
#include <stdio.h>

#include <host/common/tsys_errors.h>
#include <host/file_utils.h>
#include <host/port.h>
#include <host/weather.h>
#include <thermostat/protocol.h>
#include <thermostat/weather_condit.h>

#include "weather_client.h"

static int16_t float_to_int(float value)
{
    if (value > 0.0f) {
        return (int16_t) 100.0f * value + 0.5f;
    } else {
        return (int16_t) 100.0f * value - 0.5f;
    }
}

static void construct_weather_packet(
    struct data_packet *pkt,
    struct weather_data *weather)
{
    if (!pkt) {
        return;
    }

    pkt->start_byte = START_BYTE;
    pkt->type = WEATHER;
    pkt->length = 3;

    // Store temp in packet
    int16_t temp_scaled = float_to_int(weather->temp);
    pkt->payload[0] = (uint8_t) (temp_scaled >> 8);
    pkt->payload[1] = (uint8_t) (temp_scaled & 0xFF);

    // Store condition in packet
    pkt->payload[2] = (uint8_t) weather->condit;

    pkt->checksum = 3;
}

static enum tsys_err send_temp(float temp)
{
    struct data_packet pkt = {0};
    pkt.start_byte = START_BYTE;
    pkt.type = TEMP;
    pkt.length = 2;

    int16_t temp_scaled = float_to_int(temp);
    pkt.payload[0] = (uint8_t) (temp_scaled >> 8);
    pkt.payload[1] = (uint8_t) (temp_scaled & 0xFF);

    pkt.checksum = 2;

    if (port_send_packet(&pkt) < 0) {
        return TSYS_E_PORT;
    }

    return TSYS_OK;
}

static enum tsys_err send_condition(enum weather_condit condit)
{
    struct data_packet pkt = {0};
    pkt.start_byte = START_BYTE;
    pkt.type = CONDITION;
    pkt.length = 1;

    pkt.payload[0] = condit;
    pkt.checksum = 1;

    if (port_send_packet(&pkt) < 0) {
        return TSYS_E_PORT;
    }

    return TSYS_OK;
}

static const char *condit_tostr(enum weather_condit condit)
{
    switch (condit) {
    case CONDIT_CLEAR:
        return "Clear";

    case CONDIT_CLOUDY:
        return "Cloudy";

    case CONDIT_RAINING:
        return "Raining";

    case CONDIT_SNOWING:
        return "Snowing";

    default:
        return "Unknown";
    }
}

enum tsys_err home_store_temp(
    int temp_fd,
    const struct data_packet *pkt)
{
    if (!pkt) {
        return TSYS_E_INVAL;
    }

    const int16_t value =
        (int16_t) (((uint16_t) pkt->payload[0] << 8) |
                   pkt->payload[1]);
    const float temp = value / 100.0f;
    printf("Temp received: %.2f\n", temp);

    // Convert temp to string
    char temp_string[16];
    const int len =
        snprintf(temp_string, sizeof(temp_string), "%.2f", temp);

    // Seek to end of file and write line
    if (file_seek(temp_fd, END) < 0) {
        return TSYS_E_FILE;
    }

    if (file_write_line(temp_fd, temp_string, len) <= 0) {
        return TSYS_E_FILE;
    }

    return TSYS_OK;
}

enum tsys_err home_send_weather(
    const struct net_device *http_dev,
    struct weather_data *weather)
{
    if (!http_dev || !weather) {
        return TSYS_E_INVAL;
    }

    // Get temp from API
    int get_temp_err = weather_client_get_temp(http_dev, weather);
    if (get_temp_err < 0) {
        return get_temp_err;
    }
    printf("Outdoor temp: %.2f\n", weather->temp);

    // Get condition from API
    int get_condition_err =
        weather_client_get_condition(http_dev, weather);
    if (get_condition_err < 0) {
        return get_condition_err;
    }

    // Convert enum to string and print
    const char *condit_string = condit_tostr(weather->condit);
    if (!condit_string) {
        return TSYS_E_INVAL;
    }

    printf("Condition: %s\n", condit_string);

    /* Construct and send weather packet */
    struct data_packet pkt = {0};
    construct_weather_packet(&pkt, weather);

    if (port_send_packet(&pkt) < 0) {
        return TSYS_E_PORT;
    }

    return TSYS_OK;
}
