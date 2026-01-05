#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <stddef.h>
/**
 * @brief Averages data
 * @param Array of floats, # of non-garbage elements
 *
 * @return Average of array
 */
float analyze_average(const float *data, size_t length);

/**
 * @brief Finds max of data
 * @param Array of floats, # of non-garbage elements
 * 
 * @return Max of array
 */
float analyze_max(const float *data, size_t length);

/**
 * @brief Finds min of data
 * @param Array of floats, # of non-garbage elements
 * 
 * @return Min of array
 */
float analyze_min(const float *data, size_t length);

#endif // ANALYSIS_H
