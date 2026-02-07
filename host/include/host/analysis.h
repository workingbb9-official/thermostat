#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <stddef.h>

/*
 * @brief Calculate average of data
 *
 * @param data - array of floats
 * @param length - # of elements in the array
 *
 * @return Average of data, or 0 if invalid params
 */
float analyze_average(const float *data, size_t length);

/*
 * @brief Find max value of data
 *
 * @param data - array of floats
 * @param length - # of elements in the array
 *
 * @return Max value of data, or 0 if invalid params
 */
float analyze_max(const float *data, size_t length);

/*
 * @brief Find min value of data
 *
 * @param data - array of floats
 * @param length - # of elements in the array
 *
 * @return Min value of data, or 0 if invalid params
 */
float analyze_min(const float *data, size_t length);

#endif // ANALYSIS_H
