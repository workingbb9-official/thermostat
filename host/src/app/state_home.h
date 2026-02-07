#ifndef STATE_HOME_H
#define STATE_HOME_H

#include <host/common/tsys_errors.h>

struct data_packet;
struct net_device;
struct weather_data;

/**
 * @brief Convert temp payload into float and store
 *
 * @param pkt - Raw data packet holding temp in payload[0] and [1]
 * @param temp_fd - File desc of the temperature file
 *
 * @retval TSYS_OK - Temp was stored
 * @retval TSYS_E_INVAL - pkt was NULL
 * @retval TSYS_E_FILE - Failed to write temp to file
 */
enum tsys_err home_store_temp(
    int temp_fd,
    const struct data_packet *pkt);

/**
 * @brief Get weather data and send to firmware
 *
 * Outdoor temp is sent in a packet.
 * It is split into payload[0] and [1].
 *
 * @param http_dev - Net device for fetching from http API
 * @param weather - Weather data struct to hold and send weather
 *
 * @retval TSYS_OK - Weather was collected and sent
 * @retval TSYS_E_INVAL - http_dev or weather was NULL
 * @retval TSYS_E_WEATHER - Failed to get or send weather
 */
enum tsys_err home_send_weather(
    const struct net_device *http_dev,
    struct weather_data *weather);

#endif // STATE_HOME_H
