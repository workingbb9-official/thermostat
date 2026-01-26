#ifndef SYSTEM_CORE_H
#define SYSTEM_CORE_H

#define TSYS_OK           0
#define TSYS_E_PORT      -1
#define TSYS_E_STORAGE   -2
#define TSYS_E_SIGNAL    -3
#define TSYS_E_ANALYZE   -4
#define TSYS_E_NETWORK   -5
#define TSYS_E_WEATHER   -6
#define TSYS_E_FILE      -7

/*
 * @brief Initialize all managers and sigint
 *
 * This function will exit the program if init fails
 * It will printf() the specific errors
 *
 * @return TSYS_OK, TSYS_PORT_ERROR, TSYS_STORAGE_ERROR
 */
int system_init(void);

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
int system_cleanup(void);

/*
 * @brief Analyze all current storage data
 *
 * This function must be called after system_init()
 * It requires the storage manager
 * It will print average, max, and min
 *
 * @return TSYS_OK, TSYS_ANALYZE_ERROR
 */
int system_analyze(void);

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
