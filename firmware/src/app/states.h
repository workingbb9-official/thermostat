#ifndef STATES_H
#define STATES_H

#ifndef NULL
#define NULL ((void *) 0)
#endif

struct state_ops {
    void (*init)(void);
    void (*keypress)(void);
    void (*process)(void);
    void (*display)(void);
    void (*send)(void);
    void (*receive)(void);
    void (*exit)(void);
};

extern const struct state_ops state_login;
extern const struct state_ops state_home;
extern const struct state_ops state_stats;

/**
 * @brief Handle transition of state
 *
 * This will call the exit of the current state.
 * Then it calls the init of the new state.
 * Current state is updated to be the new state.
 *
 * @param *new_state -- State to transition to
 */
void sys_change_state(const struct state_ops *new_state);

#endif // STATES_H
