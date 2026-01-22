#include <host/weather.h> 
#include <stdio.h>

int main(void) {
    const char *raw_json = 
        "{"
        "  \"current\": {"
        "    \"time\": \"2024-03-20T12:00\","
        "    \"interval\": 900,"
        "    \"temperature\": 15.4"
        "  }"
        "}";
    
    struct weather_device dev = {0};
    weather_get_temp(raw_json, &dev);

    printf("Temp: %f\n", dev.temp);
    return 0;
}
