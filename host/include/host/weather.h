#ifndef WEATHER_H
#define WEATHER_H

struct weather_device {
    float temp;
};

enum json_status {
    JSON_OK = 0,
    JSON_ERR_ROOT,
    JSON_ERR_CUR,
    JSON_ERR_TEMP
};

enum json_status weather_get_temp(const char *raw_json, 
                                  struct weather_device *dev);

#endif // WEATHER_H
