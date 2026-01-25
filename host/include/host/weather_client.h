#ifndef WEATHER_CLIENT_H
#define WEATHER_CLIENT_H

#define WEATHER_CLIENT_OK            0
#define WEATHER_CLIENT_E_INVAL      -1
#define WEATHER_CLIENT_E_NET        -2
#define WEATHER_CLIENT_E_WEATHER    -3
#define WEATHER_CLIENT_E_PORT       -4

/* Forward declare */
struct net_device;
struct weather_data;

/**
 * @brief Get temp from an online api
 *
 * This function depends on how weather_get_temp parses the JSON.
 * Make sure the API being used returns a JSON in the same format.
 *
 * @param *dev          Network device to fetch raw json
 * @param *data_out     Weather data struct to store final value
 * @return int          WEATHER_CLIENT error code
 */
int weather_client_get_temp(const struct net_device *dev,
                            struct weather_data *data_out);

/**
 * @brief Send weather data to the firmware
 *
 * This function will store weather data to a data packet.
 * Then it will send the data packet to the firmware.
 *
 * Temperature is stored in payload[0] and [1].
 *
 * @param *data     Weather data to be transmitted
 * @return          WEATHER_CLIENT error code
 */
int weather_client_send_weather(const struct weather_data *data);

#endif // WEATHER_CLIENT_H
