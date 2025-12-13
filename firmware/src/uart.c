#include "uart.h"

#include "config.h"

#include <stdint.h>
#include <avr/io.h>


#define UBRR_VALUE (F_CPU / (16UL * BAUD) - 1)

void uart_init(void) {
    // Set baud rate
	UBRR0H = (uint8_t)(UBRR_VALUE >> 8);
	UBRR0L = (uint8_t)UBRR_VALUE;

	UCSR0B = (1 << RXEN0) | (1 << TXEN0);   // Enable transmit and receive

    // Set data settings (1 stop bit, no parity, 8 bits)
    UCSR0C = (1 << UCSZ00);
    UCSR0C = (1 << UCSZ01);
    UCSR0B &= ~(1 << UCSZ02);
}

/**
 * @brief Transmits char over UART
 * @param Character to transmit
 *
 */
static void uart_transmit_c(const uint8_t data) {
	while (!(UCSR0A & (1 << UDRE0)));   // Wait for buffer to clear
	UDR0 = data;
}


void uart_transmit(const char *data) {
	while (*data != '\0') {
        uart_transmit_c(*data);
		++data;
	}
}
