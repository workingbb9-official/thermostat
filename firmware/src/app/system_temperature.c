#include "app/system_temperature.h"

#include "thermistor/thermistor.h"

static int16_t remove_decimal(float x);

void system_temperature_init(void) {
    therm_mgr_init();
}

int16_t system_get_temp(void) {
    const float temp_c = therm_mgr_get_temp();
    const int16_t temp_int = remove_decimal(temp_c);
    return temp_int;
}

static int16_t remove_decimal(float x) {
    if (x >= 0.0f) {
        return (int16_t) 100.0f * x + 0.5f;
    } else {
        return (int16_t) 100.0f * x - 0.5f;
    }
}
