#include <firmware/thermistor.h>

#include <math.h>
#include <stdint.h>

#include "therm_hal.h"

// Steinhart-hart coefficients
#define A 0.001129148
#define B 0.000234125
#define C 0.0000000876741

static float calc_resistance(uint16_t reading)
{
    const float ratio = (float) reading / 1023.0f;
    const float resistance = 10000.0f * ratio / (1.0f - ratio);
    return resistance;
}

static float calc_temp(float resistance)
{
    const float r = resistance;
    // Steinhart-hart equation to get temp from resistance
    const float temp_k =
        1.0f / (A + B * log(r) + C * log(r) * log(r) * log(r));
    const float temp_c = temp_k - 273.15f;
    return temp_c;
}

void therm_init(void)
{
    therm_hal_init();
}

float therm_get_temp(void)
{
    const uint16_t reading = therm_hal_read();
    const float resistance = calc_resistance(reading);
    return calc_temp(resistance);
}
