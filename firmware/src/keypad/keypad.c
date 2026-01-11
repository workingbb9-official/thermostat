#include <firmware/keypad.h>

#include <avr/io.h>
#include <util/delay.h>

#include <firmware/common/bit_utils.h>

static int8_t check_rows(const uint8_t *row_bits);

const char key_map[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

void keypad_init(void) {
	// Col type 
	SET_BIT(DDRB, DDB1);
	SET_BIT(DDRB, DDB2);
	SET_BIT(DDRB, DDB3);
	SET_BIT(DDRB, DDB4);

	// Col state
	SET_BIT(PORTB, PORTB1);
	SET_BIT(PORTB, PORTB2);
	SET_BIT(PORTB, PORTB3);
	SET_BIT(PORTB, PORTB4);
	
	// Row type 
	CLR_BIT(DDRB, DDB0);
	CLR_BIT(DDRD, DDD5);
	CLR_BIT(DDRD, DDD6);
	CLR_BIT(DDRD, DDD7);

	// Row state
	SET_BIT(PORTB, PORTB0);
	SET_BIT(PORTD, PORTD5);
	SET_BIT(PORTD, PORTD6);
	SET_BIT(PORTD, PORTD7);

    _delay_ms(20); // Time to settle
}

char keypad_read(void) {
    static const uint8_t col_bits[COLS] = {PORTB1, PORTB2, PORTB3, PORTB4};
    
    // Omit a row because it sits on a diff reg (pinb)
    static const uint8_t row_bits[ROWS - 1] = {PIND5, PIND6, PIND7};

    for (uint8_t col = 0; col < COLS; ++col) {
        CLR_BIT(PORTB, col_bits[col]);

        int8_t pressed_row = check_rows(row_bits);
        if (pressed_row != -1) {
            SET_BIT(PORTB, col_bits[col]);
            return key_map[pressed_row][col];
        }
        
        // Check the other one
        if (READ_BIT(PINB, PINB0)) {
            SET_BIT(PORTB, col_bits[col]);
            continue;
        }
        _delay_ms(20);
        if (READ_BIT(PINB, PINB0)) {
            SET_BIT(PORTB, col_bits[col]);
            continue;
        }

        SET_BIT(PORTB, col_bits[col]);
        return key_map[3][col];
    }

    return NO_KEY;
}

static int8_t check_rows(const uint8_t *row_bits) {
    for (int8_t row = 0; row < ROWS - 1; ++row) {
        // Skip if high (not pressed)
        if (READ_BIT(PIND, row_bits[row])) {
            continue;
        }
        _delay_ms(20); // Debounce
        if (READ_BIT(PIND, row_bits[row])) {
            continue;
        }

        return row;
    }

    return -1;
}
