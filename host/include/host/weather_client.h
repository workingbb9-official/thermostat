#ifndef WEATHER_CLIENT_H
#define WEATHER_CLIENT_H

#define WEATHER_CLIENT_OK            0
#define WEATHER_CLIENT_E_NET        -1
#define WEATHER_CLIENT_E_WEATHER    -2

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

#endif // WEATHER_CLIENT_H
