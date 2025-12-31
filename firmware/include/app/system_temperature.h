#ifndef SYSTEM_TEMPERATURE_H
#define SYSTEM_TEMPERATURE_H

/*
 * @brief Handles sending temperature to host at intervals
 *
 * Creates a temperature packet and sends it with UART manager
 * Turns float into int and splits that into (uint8_t) high byte, low byte
 * Checksum not implemented yet (same as length)
 * Arbitrary timer boundary set (around 5 seconds)
 *
 */
void system_send_temp(void);

#endif // SYSTEM_TEMPERATURE_H
