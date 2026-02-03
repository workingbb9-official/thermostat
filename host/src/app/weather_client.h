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
 *      dev: Network device to fetch JSON from web
 *      weather_out: Weather data struct to store temp
 *
 * Return:
 *      TSYS_OK: Temp retrieved and stored
 *      TSYS_E_INVAL: dev or weather_out was NULL
 *      TSYS_E_NET: Failed to fetch or got invalid JSON
 *      TSYS_E_WEATHER: Failed to parse JSON for temp
 *
 * Notes:
 *      Network device must have host and path compatible with its ops.
 *      For example, https website cannot be mixed with http_ops.
 *      API being used must be compatible with weather_get_temp().
 *      See weather module for specific format.
 */
enum tsys_err weather_client_get_temp(
    const struct net_device *dev,
    struct weather_data *weather_out
);

/**
 * // WIP //
 *
 * Desc: Get weather condition from an online API
 *
 * Params:
 *      dev: Network device to fetch JSON from web
 *      weather_out: Weather data struct to store condition
 *
 * Return:
 *      TSYS_OK: Condition retrieved and stored
 *      TSYS_E_INVAL: dev or weather_out was NULL
 *      TSYS_E_NET: Failed to fetch or got invalid JSON
 *      TSYS_E_WEATHER: Failed to parse JSON for condition
 *
 * Notes:
 *      Everything from weather_client_get_temp() applies here.
 *      Conditions are stored in weather_out as WEATHER_CONDIT enums.
 *      See weather module for specifics.
 */
enum tsys_err weather_client_get_condition(
    const struct net_device *dev,
    struct weather_data *weather_out
);

#endif // WEATHER_CLIENT_H
