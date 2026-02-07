#ifndef WEATHER_CONDIT_H
#define WEATHER_CONDIT_H

enum weather_condit {
    CONDIT_CLEAR = 0,
    CONDIT_CLOUDY,
    CONDIT_RAINING,
    CONDIT_SNOWING
} __attribute__((packed));

#endif // WEATHER_CONDIT_H
