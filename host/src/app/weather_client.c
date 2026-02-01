#include "weather_client.h"

#include <string.h>
#include <stdint.h>

#include <host/network.h>
#include <host/weather.h>
#include <host/port.h>
#include <host/common/tsys_errors.h>
#include <thermostat/protocol.h>

#define BUF_SIZE 2048

static const char *find_json(const char *full_response);

enum tsys_err weather_client_get_temp(
    const struct net_device *dev,
    struct weather_data *weather_out)
{
    if (!dev || !weather_out) {
        return TSYS_E_INVAL;
    }

    // Fetch response 
    char buf[BUF_SIZE];
    if (net_dev_fetch(dev, buf, BUF_SIZE) < 0) {
        return TSYS_E_NET;
    }

    // Remove header and get raw JSON
    const char *raw_json = find_json(buf);
    if (!raw_json) {
        return TSYS_E_NET;
    }

    // Parse JSON and store temp in weather_out 
    if (weather_get_temp(raw_json, weather_out) < 0) {
        return TSYS_E_WEATHER;
    }

    return TSYS_OK;
}

enum tsys_err weather_client_get_condition(
    const struct net_device *dev,
    struct weather_data *weather_out)
{
    if (!dev || !weather_out) {
        return TSYS_E_INVAL;
    }

    return TSYS_OK;
}

static const char *find_json(const char *full_response) {
    const char *body_start = strstr(full_response, "\r\n\r\n");
    if (!body_start) {
        return NULL;
    }

    body_start += 4;

    const char *json_start = strchr(body_start, '{');
    return json_start;
}
