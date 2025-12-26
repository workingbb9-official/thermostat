#ifndef SYSTEM_CORE_H
#define SYSTEM_CORE_H

/*
 * @brief Initializes all managers
 *
 */
void system_init(void);

/*
 * @brief Sends temp from therm over UART to host
 *
 */
void system_send_temp(void);

#endif // SYSTEM_CORE_H
