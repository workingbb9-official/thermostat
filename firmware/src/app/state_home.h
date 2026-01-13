#ifndef STATE_HOME_H
#define STATE_HOME_H

#include "states.h"

/*
 * @brief Handle logic for the home/menu
 * @param Pointer to current system state
 *
 * This function will read temp from thermistor
 * It will format that data and send it to host
 * Press '#' to go to stats screen
 *
 */
void home_run(enum sys_state *current_state);
#endif // STATE_HOME_H
