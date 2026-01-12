#include "uart_hal.h"

#include <avr/io.h>

#include <firmware/common/board_config.h>
#include <util/setbaud.h>
#include <firmware/common/bit_utils.h>

void uart_init(void) {
    // Set baud rate
	UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;

    #if USE_2X
        SET_BIT(UCSR0A, U2X0); // Double speed mode if required
    #endif

	UCSR0B = (1 << RXEN0) | (1 << TXEN0);   // Enable transmit and receive

    // Set data settings (1 stop bit, no parity, 8 bits)
    UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);
    CLR_BIT(UCSR0B, UCSZ02);
}

void uart_transmit_byte(uint8_t byte) {
	while (!(UCSR0A & (1 << UDRE0)));   // Wait for buffer to clear
	UDR0 = byte;
}

uint8_t uart_receive_byte(void) {
    while (!(UCSR0A & (1 << RXC0)));
    return UDR0;
}
