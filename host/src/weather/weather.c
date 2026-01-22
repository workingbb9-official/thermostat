#include <host/weather.h>

#include "cJSON.h"

enum json_status weather_get_temp(const char *raw_json,
                                  struct weather_device *dev)
{
    cJSON *root = cJSON_Parse(raw_json);
    if (!root)
        return JSON_ERR_ROOT;

    cJSON *current = cJSON_GetObjectItem(root, "current");
    if (!current) {
        cJSON_Delete(root);
        return JSON_ERR_CUR;
    }

    cJSON *temp = cJSON_GetObjectItem(current, "temperature");
    if (!cJSON_IsNumber(temp)) {
        cJSON_Delete(root);
        return JSON_ERR_TEMP;
    }

    dev->temp = temp->valuedouble;
    
    cJSON_Delete(root);
    return JSON_OK;
}
