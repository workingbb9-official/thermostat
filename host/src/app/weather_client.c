#include <host/weather_client.h>

#include <string.h>
#include <stdint.h>

#include <host/network.h>
#include <host/weather.h>
#include <host/port.h>
#include <thermostat/protocol.h>

#define BUF_SIZE 2048

static const char *find_json(const char *full_response);
static int16_t float_to_int(float value);

int weather_client_get_temp(const struct net_device *dev,
                            struct weather_data *data_out)
{
    if (!dev || !data_out)
        return WEATHER_CLIENT_E_INVAL;

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

int weather_client_send_weather(const struct weather_data *data) {
    if (!data)
        return WEATHER_CLIENT_E_INVAL;
    
    struct data_packet pkt = {0};
    pkt.start_byte = START_BYTE;
    pkt.type = HOME;
    pkt.length = 2;

    int16_t temp_scaled = float_to_int(data->temp);
    pkt.payload[0] = (uint8_t) (temp_scaled >> 8);
    pkt.payload[1] = (uint8_t) (temp_scaled & 0xFF);

    pkt.checksum = 2;

    if (port_mgr_send_packet(&pkt) < 0)
        return WEATHER_CLIENT_E_PORT;

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

static int16_t float_to_int(float value) {
    if (value > 0.0f)
        return (int16_t) 100.0f * value + 0.5f;
    else
        return (int16_t) 100.0f * value - 0.5f;
}
