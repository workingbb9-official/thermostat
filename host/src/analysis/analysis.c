#include <host/analysis.h>

float analyze_average(const float *data, size_t length) {
    float total = 0.0f;
    for (size_t i = 0; i < length; ++i) {
        total += data[i];
    }
    return total / length;
}

float analyze_max(const float *data, size_t length) {
    float max = -89.2f;
    for (size_t i = 0; i < length; ++i) {
        if (data[i] > max) {
            max = data[i];
        }
    }
    return max;
}

float analyze_min(const float *data, size_t length) {
    float min = 56.7f;
    for (size_t i = 0; i < length; ++i) {
        if (data[i] < min) {
            min = data[i];
        }
    }
    return min;
}
