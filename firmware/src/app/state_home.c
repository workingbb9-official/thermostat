#include <stdint.h>
#include <avr/pgmspace.h>

#include <firmware/thermistor.h>
#include <firmware/uart.h>
#include <firmware/lcd.h>
#include <firmware/keypad.h>
#include <thermostat/protocol.h>
#include "states.h"
#include "strings.h"

#define HOME_DELAY_TICKS 125000UL
#define HOME_KEY_STATS   '#'

static struct {
    uint32_t ticks;
    char input;

    struct {
        int16_t temp;
        struct data_packet temp_packet;
    } indoor;
    
    struct {
        int16_t temp;
    } weather;
    
    struct rx_ctx rx;
    struct data_packet rx_pkt;

    union {
        uint8_t all;
        struct {
            uint8_t lcd_dirty       : 1;
            uint8_t tx_req          : 1;
            uint8_t rx_req          : 1;

            uint8_t tx_complete     : 1;
            uint8_t rx_complete     : 1;
            uint8_t input_pending   : 1;

            uint8_t reserved        : 2;
        }; 
    } flags;
} home_ctx;

static void home_init(void);
static void home_keypress(void);
static void home_process(void);
static void home_display(void);
static void home_send(void);
static void home_receive(void);

const struct state_ops state_home = {
    .init     = home_init,
    .keypress = home_keypress,
    .display  = home_display,
    .process  = home_process,
    .send     = home_send,
    .receive  = home_receive,
    .exit     = NULL
};

static int16_t format_temp(float temp);
static void configure_temp_packet(void);

static void home_init(void) {
    home_ctx.ticks = HOME_DELAY_TICKS;
    home_ctx.rx.stage = 0;
    home_ctx.rx.payload_idx = 0;
    home_ctx.flags.all = 0;

    uart_clear_rx();
}

static void home_keypress(void) {
    const struct keypad_state keypad = keypad_get_state();
    if (keypad.current_key == KEYPAD_NO_KEY ||
        keypad.current_key == keypad.last_key)
        return;
    
    home_ctx.flags.input_pending = 1;
    home_ctx.input = keypad.current_key;
}

static void home_process(void) {
    if (home_ctx.flags.input_pending) {
        home_ctx.flags.input_pending = 0;

        switch (home_ctx.input) {
        case HOME_KEY_STATS:
            sys_change_state(&state_stats);
            return;
        default:
            break;
        }
    }

    // After transmit, expect return
    if (home_ctx.flags.tx_complete) {
        home_ctx.flags.tx_complete = 0;
        home_ctx.flags.rx_req = 1;
    }

    if (home_ctx.flags.rx_complete) {
        home_ctx.flags.rx_complete = 0;
        home_ctx.flags.lcd_dirty = 1;
    }

    if (++home_ctx.ticks < HOME_DELAY_TICKS)
        return;

    home_ctx.ticks = 0;
    
    // Update temp
    const float raw_temp = therm_get_temp();
    home_ctx.indoor.temp = format_temp(raw_temp);
    
    // Display and send temp
    home_ctx.flags.lcd_dirty = 1;
    home_ctx.flags.tx_req = 1;
}

static void home_display(void) {
    if (!home_ctx.flags.lcd_dirty)
        return;

    home_ctx.flags.lcd_dirty = 0;

    // Indoor temp
    lcd_clear();
    lcd_draw_pstr(PSTR("In: "));
    lcd_draw_int(home_ctx.indoor.temp / 100);
    lcd_draw_pstr(dot);
    lcd_draw_int(home_ctx.indoor.temp % 100);
    lcd_draw_pstr(degrees_c);
    
    // Outdoor temp
    lcd_set_cursor(1, 0);
    lcd_draw_pstr(PSTR("Out: "));
    lcd_draw_int(home_ctx.weather.temp / 100);
    lcd_draw_pstr(dot);
    
    // Keep low part positive to prevent -15.-70
    int16_t decimal = home_ctx.weather.temp % 100;
    if (decimal < 0)
        decimal = -decimal;

    lcd_draw_int(decimal);
    lcd_draw_pstr(degrees_c);
}

static void home_send(void) {
    if (!home_ctx.flags.tx_req)
        return;

    home_ctx.flags.tx_req = 0;
    home_ctx.flags.tx_complete = 1;
    
    configure_temp_packet();
    uart_send_packet(&home_ctx.indoor.temp_packet);
}

static void home_receive(void) {
    if (!home_ctx.flags.rx_req)
        return;
    
    struct data_packet *pkt = &home_ctx.rx_pkt;
    int8_t rx_status = uart_receive_packet(&home_ctx.rx, pkt);

    // Check for error
    if (rx_status < 0 && rx_status != UART_INCOMPLETE) {
        home_ctx.rx.stage = 0;
        home_ctx.rx.payload_idx = 0;
        return;
    }
    
    // If incomplete, try again next loop
    if (rx_status == UART_INCOMPLETE)
        return;

    // Store temp
    home_ctx.weather.temp =
        (int16_t) (((uint16_t) pkt->payload[0] << 8) | pkt->payload[1]);

    // Reset for the next packet
    home_ctx.rx.stage = 0;
    home_ctx.rx.payload_idx = 0;

    home_ctx.flags.rx_req = 0;
    home_ctx.flags.rx_complete = 1;
}

static int16_t format_temp(float temp) {
    if (temp > 0.0f) {
        return (int16_t) 100.0f * temp + 0.5f;
    } else {
        return (int16_t) 100.0f * temp - 0.5f;
    }
}

static void configure_temp_packet(void) {
    struct data_packet *packet = &home_ctx.indoor.temp_packet;

    packet->start_byte = START_BYTE;
    packet->type = HOME; 
    packet->length = 2;

    packet->payload[0] = (uint8_t) (home_ctx.indoor.temp >> 8);
    packet->payload[1] = (uint8_t) (home_ctx.indoor.temp & 0xFF);

    packet->checksum = 2;
}
