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

    // Get weather_condition item from current
    cJSON *code = cJSON_GetObjectItem(current, "weather_code");
    if (!cJSON_IsNumber(code)) {
        cJSON_Delete(root);
        return WEATHER_E_JSON;
    }

    int weather_code = code->valueint;
    switch (weather_code) {
    case 0:
    case 1:
        weather_out->condit = CONDIT_CLEAR;
        break;
    case 2:
    case 3:
    case 45:
    case 48:
        weather_out->condit = CONDIT_CLOUDY;
        break;
    case 51:
    case 53:
    case 55:
    case 56:
    case 57:
    case 61:
    case 63:
    case 65:
    case 66:
    case 67:
    case 80:
    case 81:
    case 82:
        weather_out->condit = CONDIT_RAINING;
        break;
    case 71:
    case 73:
    case 75:
    case 77:
    case 85:
    case 86:
        weather_out->condit = CONDIT_SNOWING;
        break;
    default:
        weather_out->condit = CONDIT_CLEAR;
        break;
    }

    cJSON_Delete(root);

    return WEATHER_OK;
}
