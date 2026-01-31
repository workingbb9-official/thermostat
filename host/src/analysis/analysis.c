#include <host/analysis.h>

float analyze_average(const float *data, size_t length) {
    if (!data || length == 0)
        return 0.0f;

    float total = 0.0f;
    for (size_t i = 0; i < length; ++i) {
        total += data[i];
    }

    return total / length;
}

float analyze_max(const float *data, size_t length) {
    if (!data || length == 0)
        return 0.0f;

    // Start at min temp so value is always found
    float max = -89.2f;
    for (size_t i = 0; i < length; ++i) {
        if (data[i] > max) {
            max = data[i];
        }
    }

    return max;
}

float analyze_min(const float *data, size_t length) {
    if (!data || length == 0)
        return 0.0f;
    
    // Start at max temp so value is always found 
    float min = 56.7f;
    for (size_t i = 0; i < length; ++i) {
        if (data[i] < min) {
            min = data[i];
        }
    }

    return min;
}
