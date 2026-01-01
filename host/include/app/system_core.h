#ifndef SYSTEM_CORE_H
#define SYSTEM_CORE_H

typedef enum {
    TSYS_OK = 0,
    TSYS_PORT_ERROR = 1,
    TSYS_STORAGE_ERROR = 2,
    TSYS_SIGNAL_ERROR = 3,
    TSYS_ANALYZE_ERROR = 4
} ThermStatus;

/*
 * @brief Initialize all managers and sigint
 *
 * This function will exit the program if init fails
 * It will printf() the specific errors
 *
 * @return TSYS_OK, TSYS_PORT_ERROR, TSYS_STORAGE_ERROR
 */
ThermStatus system_init(void);

/*
 * @brief Run all system operations
 *
 * This function will receive data with system receiver
 * Then it will give it over to the system handler
 * It will printf() and exit if errors come up
 *
 */
void system_run(void);

/*
 * @brief Destroy all objects
 *
 * This function exits the program
 * It will printf() errors if they come up
 *
 * @return TSYS_OK, TSYS_PORT_ERROR, TSYS_STORAGE_ERROR
 */
ThermStatus system_cleanup(void);

/*
 * @brief Analyze all current storage data
 *
 * This function must be called after system_init()
 * It requires the storage manager
 * It will print average, max, and min
 *
 * @return TSYS_OK, TSYS_ANALYZE_ERROR
 */
ThermStatus system_analyze(void);

/*
 * @brief Tell main when to shutdown
 *
 * This function will be true on signal interrupt
 * Used to exit the program with CTRL-C
 *
 * @return 0 for no, 1 for shutdown
 */
int system_should_shutdown(void);

#endif // SYSTEM_CORE_H
