#ifndef WEATHER_CLIENT_H
#define WEATHER_CLIENT_H

#include <host/common/tsys_errors.h>

/* Forward declare */
struct net_device;
struct weather_data;

/**
 * Desc: Get temp from an online api
 *
 * Params:
 *      dev: Device to fetch JSON from web
 *      data_out: Weather data struct to store temp
 *
 * Return:
 *      TSYS_OK: Temp retrieved and stored
 *      TSYS_E_INVAL: dev or data_out was NULL
 *      TSYS_E_NET: Failed to fetch or invalid API
 *      TSYS_E_WEATHER: Failed to parse JSON
 *
 * Notes:
 *      Network device must have host and path compatible with its ops.
 *      For example, https website cannot be mixed with http_ops.
 *      API being used must be compatible with weather_get_temp().
 *      See weather module for specific format.
 */
enum tsys_err weather_client_get_temp(
    const struct net_device *dev,
    struct weather_data *data_out
);

/**
 * Desc: Send weather data to the firmware
 *
 * Params:
 *      data: Weather data to transmit
 *
 * Return:
 *      TSYS_OK: Transmitted data
 *      TSYS_E_INVAL: data was NULL
 *      TSYS_E_PORT: Failed to send packet
 *
 * Notes:
 *      Data is parsed into a data packet to transmit.
 *      Temperature is stored in payload[0] and [1].
 */
enum tsys_err weather_client_send_weather(
    const struct weather_data *data
);

#endif // WEATHER_CLIENT_H
