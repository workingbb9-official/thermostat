#include "uart.h"

#include <stdint.h>
#include <avr/io.h>

#include "config.h"
#include <util/setbaud.h>

void uart_init(void) {
    // Set baud rate
	UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;

    #if USE_2X
        UCSR0A |= (1 << U2X0);
    #endif

	UCSR0B = (1 << RXEN0) | (1 << TXEN0);   // Enable transmit and receive

    // Set data settings (1 stop bit, no parity, 8 bits)
    UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);
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

    uart_transmit_c('\0');
}
