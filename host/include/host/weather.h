#ifndef WEATHER_H
#define WEATHER_H

// WEATHER errors run from -59 to -50
enum weather_err {
    WEATHER_OK = 0,
    WEATHER_E_INVAL = -59,
    WEATHER_E_NOMEM,
    WEATHER_E_JSON = -57
};

struct weather_data {
    float temp;
};

/**
 * Desc: Parse raw json and store temperature in weather data struct
 *
 * Params:
 *      raw_json: JSON in the format shown below
 *      data_out: Weather data struct to store temperature
 *
 * Return:
 *      WEATHER_OK: Temp was found and stored
 *      WEATHER_E_INVAL: raw_json or data_out was NULL
 *      WEATHER_E_NOMEM: No system mem to create cJSON item
 *      WEATHER_E_JSON: Failed to find "current" or "temperature_2m"
 *
 * Notes:
 *      This function is designed to parse something like this:
 *      "{"current":
 *          {
 *              "time":"2026-01-23T00:15",
 *              "interval":900,
 *              "temperature_2m":2.2
 *          }
 *      }"
 *
 *      JSON must contain this: "current":{"temperature_2m": x.x}.
 */
enum weather_err weather_get_temp(
    const char *raw_json,
    struct weather_data *data_out
);

#endif // WEATHER_H
