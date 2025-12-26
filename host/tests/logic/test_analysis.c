#include "logic/analysis.h"

#include <stdio.h>
#include <stddef.h>

int main(void) {
    const float data[] = {12.34, 13.45, 32.18, 18.67};
    const size_t length = sizeof(data) / sizeof(data[0]);

    const float average = analyze_average(data, length);
    printf("Average: %f\n", average);
    
    const float max = analyze_max(data, length);
    printf("Max: %f\n", max);


    const float min = analyze_min(data, length);
    printf("Min: %f\n", min);

    return 0;
}
