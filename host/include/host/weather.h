#ifndef WEATHER_H
#define WEATHER_H

#define WEATHER_OK       0 
#define WEATHER_E_INVAL  -1
#define WEATHER_E_NOMEM  -2
#define WEATHER_E_JSON   -3

struct weather_data {
    float temp;
};

/**
 * @brief Parse raw json and store temperature in weather device
 *
 * This function is designed to parse something like this
 *      "{"current":
 *          {
 *              "time":"2026-01-23T00:15",
 *              "interval":900,
 *              "temperature":2.2
 *          }
 *      }"
 *  JSON passed in must have the same "current":{"temperature": x.x} format.
 *
 * @param *raw_json     JSON containing the above key-value pair 
 * @param *data_out     Weather device to store temperature in
 * @return int          WEATHER error code
 */
int weather_get_temp(const char *raw_json, struct weather_data *data_out);

#endif // WEATHER_H
