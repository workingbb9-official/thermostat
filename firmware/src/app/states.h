#ifndef STATES_H
#define STATES_H

enum sys_state {
    STATE_LOGIN = 0,
    STATE_HOME,
    STATE_STATS
} __attribute__((packed));

#endif // STATES_H
