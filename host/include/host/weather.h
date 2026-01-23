#ifndef WEATHER_H
#define WEATHER_H

#define WEATHER_OK       0 
#define WEATHER_EINVAL  -1
#define WEATHER_ENOMEM  -2
#define WEATHER_EJSON   -3

struct weather_data {
    float temp;
};

/**
 * @brief Parse raw json and store temperature in weather device
 * @param *raw_json 
 * JSON string in the format of "root:" "current:" "temperature:"
 * @param *data_out     Weather device to store temperature in
 * @return int          WEATHER error code
 */
int weather_get_temp(const char *raw_json, struct weather_data *data_out);

#endif // WEATHER_H
