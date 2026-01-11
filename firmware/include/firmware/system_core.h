#ifndef SYSTEM_CORE_H
#define SYSTEM_CORE_H

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
