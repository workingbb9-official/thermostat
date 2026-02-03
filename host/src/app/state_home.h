#ifndef STATE_HOME_H
#define STATE_HOME_H

#include <host/common/tsys_errors.h>

/* Forward declare */
struct data_packet;
struct net_device;
struct weather_data;

/**
 * Desc: Convert temp payload into float and store
 *
 * Params:
 *      pkt: Raw data packet holding temp in payload[0] and [1]
 *      temp_fd: File desc of the temperature file
 *
 * Return:
 *      TSYS_OK: Temp was stored
 *      TSYS_E_INVAL: pkt was NULL
 *      TSYS_E_FILE: Failed to write temp to file
 */
enum tsys_err home_store_temp(int temp_fd, const struct data_packet *pkt);

/**
 * Desc: Get weather data and send to firmware
 *
 * Params:
 *      http_dev: Net device for fetching from http API
 *      weather: Weather data struct to hold and send weather
 *
 * Return:
 *      TSYS_OK: Weather was collected and sent
 *      TSYS_E_INVAL: http_dev or weather was NULL
 *      TSYS_E_WEATHER: Failed to get or send weather
 *
 * Notes:
 *      Outdoor temp is sent in a packet.
 *      It is split into payload[0] and [1].
 */
enum tsys_err home_send_weather(
    const struct net_device *http_dev,
    struct weather_data *weather
);

#endif // STATE_HOME_H
