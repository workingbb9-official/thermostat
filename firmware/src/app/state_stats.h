#ifndef STATE_STATS_H
#define STATE_STATS_H

#include "states.h"

/*
 * @brief Display stats
 *
 * INCOMPLETE
 * This function only writes average
 * Host should send average as payload[0] and payload[1]
 * Press 'A' to go back home
 *
 */
void stats_run(enum sys_state *current_state);

#endif // STATE_STATS_H
