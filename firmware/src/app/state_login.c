#include <avr/pgmspace.h>
#include <stdint.h>

#include <firmware/keypad.h>
#include <firmware/lcd.h>
#include <firmware/uart.h>
#include <thermostat/protocol.h>

#include "states.h"

#define LOGIN_PWD_LEN 4

static struct {
    struct {
        char buf[LOGIN_PWD_LEN + 1];
        uint8_t idx;
    } user_pwd;
    char input;

    struct {
        struct rx_ctx ctx;
        struct data_packet pkt;
    } rx;

    uint8_t login_valid : 1;

    union {
        uint8_t all;
        struct {
            uint8_t lcd_dirty   : 1;
            uint8_t auth_failed : 1;
            uint8_t tx_req      : 1;
            uint8_t rx_req      : 1;

            uint8_t input_pending : 1;
            uint8_t tx_complete   : 1;
            uint8_t rx_complete   : 1;
            uint8_t reserved      : 1;
        };
    } flags;
} login_ctx;

static void login_init(void)
{

    login_ctx.user_pwd.buf[0] = '\0';
    login_ctx.user_pwd.idx = 0;

    login_ctx.rx.ctx.stage = 0;
    login_ctx.rx.ctx.payload_idx = 0;

    login_ctx.flags.all = 0;
    login_ctx.flags.lcd_dirty = 1;

    uart_clear_rx();
}

static void login_keypress(void)
{
    const struct keypad_state keypad = keypad_get_state();
    if (keypad.current_key == KEYPAD_NO_KEY ||
        keypad.current_key == keypad.last_key) {
        return;
    }

    login_ctx.flags.input_pending = 1;
    login_ctx.input = keypad.current_key;
}

static void login_process(void)
{
    if (login_ctx.flags.input_pending) {
        login_ctx.flags.input_pending = 0;

        // Update password and null-terminate
        login_ctx.user_pwd.buf[login_ctx.user_pwd.idx] =
            login_ctx.input;
        login_ctx.user_pwd.buf[login_ctx.user_pwd.idx + 1] = '\0';
        ++login_ctx.user_pwd.idx;

        login_ctx.flags.lcd_dirty = 1;
    }

    if (login_ctx.flags.tx_complete) {
        login_ctx.flags.tx_complete = 0;
        login_ctx.flags.rx_req = 1;
    }

    if (login_ctx.flags.rx_complete) {
        login_ctx.flags.rx_complete = 0;

        if (login_ctx.login_valid) {
            sys_change_state(&state_home);
        } else {
            login_ctx.user_pwd.buf[0] = '\0';
            login_ctx.user_pwd.idx = 0;

            login_ctx.flags.auth_failed = 1;
            login_ctx.flags.lcd_dirty = 1;
        }
    }

    if (login_ctx.user_pwd.idx != LOGIN_PWD_LEN) {
        return;
    }

    login_ctx.flags.tx_req = 1;
    login_ctx.user_pwd.idx = 0;
}

static void login_display(void)
{
    if (!login_ctx.flags.lcd_dirty) {
        return;
    }

    login_ctx.flags.lcd_dirty = 0;

    lcd_clear();
    if (login_ctx.flags.auth_failed) {
        login_ctx.flags.auth_failed = 0;

        // Message for wrong password
        lcd_draw_pstr(PSTR("Invalid password"));
        lcd_set_cursor(1, 0);
        lcd_draw_pstr(PSTR("3 more tries"));
    } else {
        // Message for waiting for input
        lcd_draw_pstr(PSTR("Enter password"));
        lcd_set_cursor(1, 0);
        lcd_draw_string(login_ctx.user_pwd.buf);
    }
}

static void login_send(void)
{
    if (!login_ctx.flags.tx_req) {
        return;
    }

    login_ctx.flags.tx_req = 0;
    login_ctx.flags.tx_complete = 1;

    struct data_packet pkt = {
        .start_byte = START_BYTE,
        .type = AUTH,
        .length = LOGIN_PWD_LEN + 1,
    };

    for (uint8_t i = 0; i < LOGIN_PWD_LEN; ++i) {
        pkt.payload[i] = login_ctx.user_pwd.buf[i];
    }

    pkt.payload[LOGIN_PWD_LEN] = '\0';
    pkt.checksum = LOGIN_PWD_LEN + 1;

    uart_send_packet(&pkt);
}

static void login_receive(void)
{
    if (!login_ctx.flags.rx_req) {
        return;
    }

    struct data_packet *pkt = &login_ctx.rx.pkt;
    int8_t rx_status = uart_receive_packet(&login_ctx.rx.ctx, pkt);

    if (rx_status < 0 && rx_status != UART_INCOMPLETE) {
        login_ctx.rx.ctx.stage = 0;
        login_ctx.rx.ctx.payload_idx = 0;
        return;
    }

    if (rx_status == UART_INCOMPLETE) {
        return;
    }

    login_ctx.login_valid = pkt->payload[0];

    login_ctx.rx.ctx.stage = 0;
    login_ctx.rx.ctx.payload_idx = 0;

    login_ctx.flags.rx_req = 0;
    login_ctx.flags.rx_complete = 1;
}

const struct state_ops state_login = {
    .init = login_init,
    .keypress = login_keypress,
    .process = login_process,
    .display = login_display,
    .send = login_send,
    .receive = login_receive,
    .exit = NULL,
};
