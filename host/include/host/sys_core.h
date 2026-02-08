#ifndef SYS_CORE_H
#define SYS_CORE_H

#define NO_SHUTDOWN 0
#define SHUTDOWN 1

#include <host/common/tsys_errors.h>

/**
 * @brief Initialize all managers and sigint
 *
 * This function will exit the program if init fails.
 * It will printf() the specific errors.
 *
 * @retval TSYS_OK - System initialized
 * @retval TSYS_PORT_ERROR - Failed to init port
 * @retval TSYS_STORAGE_ERROR - Failed to open storage files
 */
enum tsys_err sys_init(void);

/**
 * @brief Run all system operations
 *
 * This function will receive data with system receiver.
 * Then it will give it over to the system handler.
 * It will printf() and return if errors come up.
 */
void sys_run(void);

/*
 * @brief Destroy all objects
 *
 * This function closes the port and files.
 * It will printf() errors if they come up.
 *
 * @retval TSYS_OK - System was cleaned up
 * @retval TSYS_PORT_ERROR - Failed to close port
 * @retval TSYS_STORAGE_ERROR - Failed to close a storage file
 */
enum tsys_err sys_cleanup(void);

/*
 * @brief Tell main when to shutdown
 *
 * This function will be true on signal interrupt.
 * Used to exit the program with CTRL-C.
 *
 * @retval NO_SHUTDOWN - Program has not received CTRL-C
 * @retval SHUTDOWN - Program received CTRL-C
 */
int sys_should_shutdown(void);

#endif // SYS_CORE_H
