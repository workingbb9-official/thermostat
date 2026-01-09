#ifndef STATE_LOGIN_H
#define STATE_LOGIN_H

#include "app/states.h"

#define PASSWORD_LEN 4
extern const char PASSWORD[PASSWORD_LEN];

/*
 * @brief Handle logic for logging in
 * @param Pointer to current system state
 *
 * This function will poll user input
 * Once password is entered, it will verify
 * Valid will change the current state to HOME
 *
 */
void login_run(enum sys_state *current_state);

#endif // STATE_LOGIN_H
