#ifndef WEATHER_H
#define WEATHER_H

#include <thermostat/weather_condit.h>

// WEATHER errors run from -59 to -50
enum weather_err {
    WEATHER_OK = 0,
    WEATHER_E_INVAL = -59,
    WEATHER_E_NOMEM,
    WEATHER_E_JSON = -57
};

struct weather_data {
    float temp;
    enum weather_condit condit;
};

/**
 * @brief Parse json for outdoor temp and store it
 *
 * JSON must contain this: "current":{"temperature_2m": <int>.<int>}.
 * Temp is stored into weather_data.temp.
 *
 * @param raw_json - JSON in the format shown below
 * @param weather_out - Struct to store temperature
 *
 * @retval WEATHER_OK - Temp was found and stored
 * @retval WEATHER_E_INVAL - raw_json or weather_out was NULL
 * @retval WEATHER_E_NOMEM - No system mem to create cJSON item
 * @retval WEATHER_E_JSON - Failed to find "current" or
 * "temperature_2m"
 */
enum weather_err weather_get_temp(
    const char *raw_json,
    struct weather_data *weather_out);

/**
 * @brief Parse json for weather condition and store it
 *
 * JSON must contain this: "current":{"weather_code":<int>}.
 * Weather code will be stored into weather_data.condit.
 *
 * @param raw_json - JSON in format shown below
 * @param weather_out - Struct to store condition
 *
 * @retval WEATHER_OK - Condition was found and stored
 * @retval WEATHER_E_INVAL - raw_json or weather_out was NULL
 * @retval WEATHER_E_NOMEM - No system mem to create cJSON item
 * @retval WEATHER_E_JSON - Failed to find "current" or "weather_code"
 */
enum weather_err weather_get_condition(
    const char *raw_json,
    struct weather_data *weather_out);

#endif // WEATHER_H
