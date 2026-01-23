#include <host/weather.h>

#include "cJSON.h"

int weather_get_temp(const char *raw_json, struct weather_data *data_out) {
    if (!raw_json || !data_out)
        return WEATHER_EINVAL;

    cJSON *root = cJSON_Parse(raw_json);
    if (!root)
        return WEATHER_ENOMEM;

    cJSON *current = cJSON_GetObjectItem(root, "current");
    if (!current) {
        cJSON_Delete(root);
        return WEATHER_EJSON;
    }

    cJSON *temp = cJSON_GetObjectItem(current, "temperature");
    if (!cJSON_IsNumber(temp)) {
        cJSON_Delete(root);
        return WEATHER_EJSON;
    }

    data_out->temp = temp->valuedouble;
    
    cJSON_Delete(root);
    return WEATHER_OK;
}
