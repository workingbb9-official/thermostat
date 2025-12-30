#ifndef SYSTEM_TEMPERATURE_H
#define SYSTEM_TEMPERATURE_H

/*
 * @brief Handles sending temperature to host
 *
 * Creates a temperature packet and sends each byte over UART
 * Turns float into int and splits that into high byte, low byte
 *
 */
void system_send_temp(void);

#endif // SYSTEM_TEMPERATURE_H
