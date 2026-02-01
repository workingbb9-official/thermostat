#include <host/weather.h>

#include "cJSON.h"

enum weather_err weather_get_temp(
    const char *raw_json, 
    struct weather_data *weather_out) 
{
    if (!raw_json || !weather_out)
        return WEATHER_E_INVAL;
    
    // Create root JSON object
    cJSON *root = cJSON_Parse(raw_json);
    if (!root)
        return WEATHER_E_NOMEM;
    
    // Get current item from root
    cJSON *current = cJSON_GetObjectItem(root, "current");
    if (!current) {
        cJSON_Delete(root);
        return WEATHER_E_JSON;
    }

    // Get temp item from current
    cJSON *temp = cJSON_GetObjectItem(current, "temperature_2m");
    if (!cJSON_IsNumber(temp)) {
        cJSON_Delete(root);
        return WEATHER_E_JSON;
    }

    weather_out->temp = (float) temp->valuedouble;
    
    cJSON_Delete(root);
    return WEATHER_OK;
}

enum weather_err weather_get_condition(
        const char *raw_json,
        struct weather_data *weather_out)
{
    if (!raw_json || !weather_out) {
        return WEATHER_E_INVAL;
    }
    
    return WEATHER_OK;
}
