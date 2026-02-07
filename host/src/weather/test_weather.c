#include <host/weather.h>
#include <stdio.h>

int main(void)
{
    struct weather_data data = {0};

    /* Test for failure */
    const char *json_fail = {
        "{"
        " \"hiko\": {"
        "   \"InhumanReactions\": 4"
        " }"
        "}",
    };

    if (weather_get_temp(json_fail, &data) < 0) {
        printf("Failed to parse json_fail\n");
    } else {
        printf("From json_fail: %f", data.temp);
    }

    /* Test for success */
    const char *json_success = {
        "{"
        "  \"current\": {"
        "    \"time\": \"2024-03-20T12:00\","
        "    \"interval\": 900,"
        "    \"temperature_2m\": 15.4"
        "  }"
        "}",
    };

    if (weather_get_temp(json_success, &data) < 0) {
        printf("Failed to parse json_success\n");
        return 1;
    }

    printf("From json_success: %f\n", data.temp);
    return 0;
}
