#ifndef SYS_CORE_H 
#define SYS_CORE_H

/*
 * @brief Initialize all systems
 */
void sys_init(void);

/*
 * @brief Run all operations
 * 
 * This function runs on a state ops ptr
 * The state ops struct holds func ptrs
 * This function will run those func ptrs
 * States decide when to transition
 */
void sys_run(void);

#endif // SYS_CORE_H
