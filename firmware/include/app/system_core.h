#ifndef SYSTEM_CORE_H
#define SYSTEM_CORE_H

typedef enum {
    STATE_LOGIN,
    STATE_HOME,
    STATE_STATS
} SysState;

#define WRONG -1
#define CORRECT 0

#define PASSWORD_LEN 4

extern const char real_password[PASSWORD_LEN];

/*
 * @brief Initialize all systems
 *
 */
void system_init(void);

/*
 * @brief Run all operations
 * 
 * State-machine (see enum above)
 * Static functions handle each state
 *
 */
void system_run(void);

#endif // SYSTEM_CORE_H
