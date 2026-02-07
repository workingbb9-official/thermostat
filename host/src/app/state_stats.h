#ifndef STATE_STATS_H
#define STATE_STATS_H

#include <host/common/tsys_errors.h>

struct statistics {
    float avg;
    float max;
    float min;
};

/**
 * @brief Analyze temp data and find stats
 *
 * @param stats_out - Statistics buffer to update
 * @param temp_fd - File desc of the temperature file
 *
 * @retval TSYS_OK - Stats analyzed and stored
 * @retval TSYS_E_FILE - Failed to set seeker or read from file
 */
enum tsys_err stats_analyze(
    int temp_fd,
    struct statistics *stats_out);

/**
 * @brief Send stats to firmware
 *
 * Average is stored in payload[0] and [1].
 * Max is stored in [2] and [3].
 * Min is stored in [4] and [5].
 * All are in the form of [high_byte] [low_byte].
 *
 * @param stats - Statistics to send
 *
 * @retval TSYS_OK - Stats were sent
 * @retval TSYS_E_PORT - Failed to send
 */
enum tsys_err stats_send(const struct statistics *stats);

#endif // STATE_STATS_H
