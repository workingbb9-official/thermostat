#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <stddef.h>

/*
 * Calculate average of data
 *
 * Params:
 *      data:       array of floats
 *      length:     # of elements in the array
 *
 * Return:
 *      Average of data
 *      0 for invalid params
 */
float analyze_average(const float *data, size_t length);

/*
 * Find max value of data
 *
 * Params:
 *      data:       array of floats
 *      length:     # of elements in the array
 *
 * Return:
 *      Max value in data
 *      0 for invalid params
 */
float analyze_max(const float *data, size_t length);

/*
 * Find min value of data
 *
 * Params:
 *      data:       array of floats
 *      length:     # of elements in the array
 *
 * Return:
 *      Min value in data
 *      0 for invalid params
 */
float analyze_min(const float *data, size_t length);

#endif // ANALYSIS_H
