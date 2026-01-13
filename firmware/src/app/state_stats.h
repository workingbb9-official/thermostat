#ifndef STATE_STATS_H
#define STATE_STATS_H

#include "states.h"

/*
 * @brief Display stats
 * @param Pointer to current state
 *
 * Host should send:
 * Average as payload[0] and [1]
 * Max as [2] and [3]
 * Min as [4] and [5]
 *
 * Press 'A' to go to home screen
 * Press 'B' to switch from max or min
 *
 */
void stats_run(enum sys_state *current_state);

#endif // STATE_STATS_H
