#ifndef SYS_CORE_H
#define SYS_CORE_H

#define NO_SHUTDOWN 0
#define SHUTDOWN 1

#include <host/common/tsys_errors.h>

/*
 * @brief Initialize all managers and sigint
 *
 * This function will exit the program if init fails
 * It will printf() the specific errors
 *
 * @return TSYS_OK, TSYS_PORT_ERROR, TSYS_STORAGE_ERROR
 */
enum tsys_err sys_init(void);

/*
 * @brief Run all system operations
 *
 * This function will receive data with system receiver
 * Then it will give it over to the system handler
 * It will printf() and exit if errors come up
 *
 */
void sys_run(void);

/*
 * @brief Destroy all objects
 *
 * This function exits the program
 * It will printf() errors if they come up
 *
 * @return TSYS_OK, TSYS_PORT_ERROR, TSYS_STORAGE_ERROR
 */
enum tsys_err sys_cleanup(void);

/*
 * @brief Analyze all current storage data
 *
 * This function must be called after system_init()
 * It requires the storage manager
 * It will print average, max, and min
 *
 * @return TSYS_OK, TSYS_ANALYZE_ERROR
 */
int sys_analyze(void);

/*
 * @brief Tell main when to shutdown
 *
 * This function will be true on signal interrupt
 * Used to exit the program with CTRL-C
 *
 * @return 0 for no, 1 for shutdown
 */
int sys_should_shutdown(void);

#endif // SYS_CORE_H
