#include "state_home.h"

#include <stdio.h>

#include <host/file_utils.h>
#include <host/weather.h>
#include <host/common/tsys_errors.h>
#include <thermostat/protocol.h>
#include "weather_client.h"

enum tsys_err home_store_temp(int temp_fd, const struct data_packet *pkt) {
    if (!pkt) {
        return TSYS_E_INVAL;
    }

    const int16_t value = 
        (int16_t) (((uint16_t) pkt->payload[0] << 8) | pkt->payload[1]);
    const float temp = value / 100.0f;
    printf("Temp received: %.2f\n", temp);
    
    // Convert temp to string
    char temp_string[16];
    const int len = snprintf(temp_string, sizeof(temp_string), "%.2f", temp);
    
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

    if (weather_client_get_temp(http_dev, weather) < 0) {
        return TSYS_E_WEATHER;
    }

    if (weather_client_send_weather(weather) < 0) {
        return TSYS_E_WEATHER;
    }

    printf("Outdoor temp: %.2f\n", weather->temp);
    return TSYS_OK;
}
