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
 * Desc: Parse json for outdoor temp and store it
 *
 * Params:
 *      raw_json: JSON in the format shown below
 *      weather_out: Struct to store temperature
 *
 * Return:
 *      WEATHER_OK: Temp was found and stored
 *      WEATHER_E_INVAL: raw_json or weather_out was NULL
 *      WEATHER_E_NOMEM: No system mem to create cJSON item
 *      WEATHER_E_JSON: Failed to find "current" or "temperature_2m"
 *
 * Notes:
 *      JSON must contain this: "current":{"temperature_2m": <int>.<int>}.
 *      Temp is stored into weather_data.temp.
 */
enum weather_err weather_get_temp(
    const char *raw_json,
    struct weather_data *weather_out
);

/**
 * // WIP //
 *
 * Desc: Parse json for weather condition and store it
 *
 * Params:
 *      raw_json: JSON in format shown below
 *      weather_out: Struct to store condition
 *
 * Return:
 *      WEATHER_OK: Condition was found and stored
 *      WEATHER_E_INVAL: raw_json or weather_out was NULL
 *      WEATHER_E_NOMEM: No system mem to create cJSON item
 *      WEATHER_E_JSON: Failed to find "current" or "weather_code"
 *
 * Notes:
 *      JSON must contain this: "current":{"weather_code":<int>}.
 *      Weather code will be stored into weather_data.condit.
 */
enum weather_err weather_get_condition(
    const char *raw_json,
    struct weather_data *weather_out
);

#endif // WEATHER_H
