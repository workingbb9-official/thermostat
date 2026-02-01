#include <firmware/sys_core.h>

#include <firmware/thermistor.h>
#include <firmware/uart.h>
#include <firmware/lcd.h>
#include <firmware/keypad.h>
#include <thermostat/protocol.h>
#include "states.h"

static const struct state_ops *state; 

static void notify_host_login(void);
static void notify_host_logout(void);

void sys_init(void) {
    // Initialize components
    therm_init();
    uart_init();
    lcd_init();
    keypad_init();

    // Set initial state to login
    state = &state_login;
    state->init();
}

void sys_run(void) {
    // Required functions
    state->keypress();
    state->process();
    state->display();
    
    // Optional functions
    if (state->send) {
        state->send();
    }

    if (state->receive) {
        state->receive();
    }
}

void sys_change_state(const struct state_ops *new_state) {
    if (state->exit) {
        state->exit();
    }
    
    if (state->process == state_login.process) {
        // Moving from login
        notify_host_login();
    } else if (new_state->process == state_login.process) {
        // Moving to login
        notify_host_logout();
    }

    if (new_state->init) {
        new_state->init();
    }

    state = new_state;
}

static void notify_host_login(void) {
    struct data_packet login = {
        .start_byte = START_BYTE,
        .type = LOGIN,
        .length = 1,
        .payload[0] = PAYLOAD_NONE,
        .checksum = 1
    };

    uart_send_packet(&login);
}

static void notify_host_logout(void) {
    struct data_packet logout = {
        .start_byte = START_BYTE,
        .type = LOGOUT,
        .length = 1,
        .payload[0] = PAYLOAD_NONE,
        .checksum = 1
    };

    uart_send_packet(&logout);
}
