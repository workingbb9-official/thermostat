#include "drivers/uart.h"

#include <avr/io.h>

#include "drivers/board_config.h"
#include <util/setbaud.h>
#include "drivers/bit_utils.h"

void uart_init(void) {
    // Set baud rate
	UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;

    #if USE_2X
        // UCSR0A |= (1 << U2X0);
        SET_BIT(UCSR0A, U2X0);
    #endif

	UCSR0B = (1 << RXEN0) | (1 << TXEN0);   // Enable transmit and receive

    // Set data settings (1 stop bit, no parity, 8 bits)
    UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);
    // UCSR0B &= ~(1 << UCSZ02);
    CLR_BIT(UCSR0B, UCSZ02);
}

void uart_transmit_byte(uint8_t byte) {
	while (!(UCSR0A & (1 << UDRE0)));   // Wait for buffer to clear
	UDR0 = byte;
}
