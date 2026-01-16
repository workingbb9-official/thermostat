#ifndef STATES_H
#define STATES_H

enum sys_state {
    STATE_LOGIN = 0,
    STATE_HOME,
    STATE_STATS
} __attribute__((packed));

struct state_actions {
    void (*init)(void);
    void (*on_keypress)(enum sys_state *current_state);
    void (*process)(void);
    void (*display)(void);
    void (*send)(void);
    void (*receive)(void);
    void (*exit)(void);
};

extern const struct state_actions login_state;
extern const struct state_actions home_state;
extern const struct state_actions stats_state;

void sys_change_state(const struct state_actions *new_state);

#endif // STATES_H
