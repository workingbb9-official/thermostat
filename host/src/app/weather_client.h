#ifndef WEATHER_CLIENT_H
#define WEATHER_CLIENT_H

#include <host/common/tsys_errors.h>

struct net_device;
struct weather_data;

/**
 * @brief Get temp from an online api
 *
 * Network device must have host and path compatible with its ops.
 * For example, https website cannot be mixed with http_ops.
 * API being used must be compatible with weather_get_temp().
 * See weather module for specific format.
 *
 * @param dev - Network device to fetch JSON from web
 * @param weather_out - Weather data struct to store temp
 *
 * @retval TSYS_OK - Temp retrieved and stored
 * @retval TSYS_E_INVAL - dev or weather_out was NULL
 * @retval TSYS_E_NET - Failed to fetch or got invalid JSON
 * @retval TSYS_E_WEATHER - Failed to parse JSON for temp
 */
enum tsys_err weather_client_get_temp(
    const struct net_device *dev,
    struct weather_data *weather_out);

/**
 * @brief Get weather condition from an online API
 *
 * Everything from weather_client_get_temp() applies here.
 * Conditions are stored in weather_out as WEATHER_CONDIT enums.
 * See weather module for specifics.
 *
 * @param dev - Network device to fetch JSON from web
 * @param weather_out - Weather data struct to store condition
 *
 * @retval TSYS_OK - Condition retrieved and stored
 * @retval TSYS_E_INVAL - dev or weather_out was NULL
 * @retval TSYS_E_NET - Failed to fetch or got invalid JSON
 * @retval TSYS_E_WEATHER - Failed to parse JSON for condition
 */
enum tsys_err weather_client_get_condition(
    const struct net_device *dev,
    struct weather_data *weather_out);

#endif // WEATHER_CLIENT_H
