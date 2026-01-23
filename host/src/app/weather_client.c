#include <host/weather_client.h>

#include <string.h>

#include <host/network.h>
#include <host/weather.h>

#define BUF_SIZE 2048

static const char *find_json(const char *full_response);

int weather_client_get_temp(const struct net_device *dev,
                            struct weather_data *data_out)
{
    char buf[BUF_SIZE];
    if (net_dev_fetch(dev, buf, BUF_SIZE) < 0)
        return WEATHER_CLIENT_E_NET;

    const char *raw_json = find_json(buf);
    if (!raw_json)
        return WEATHER_CLIENT_E_NET;

    if (weather_get_temp(raw_json, data_out) < 0)
        return WEATHER_CLIENT_E_WEATHER;

    return WEATHER_CLIENT_OK;
}

static const char *find_json(const char *full_response) {
    const char *body_start = strstr(full_response, "\r\n\r\n");
    if (!body_start)
        return NULL;

    body_start += 4;
    
    const char *json_start = strchr(body_start, '{');
    return json_start;
}
